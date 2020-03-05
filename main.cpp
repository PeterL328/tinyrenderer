#include <cmath>
#include <limits>
#include <memory>

#include "geometry.h"
#include "tgaimage.h"
#include "model.h"


void drawLine(TGAImage *image, const TGAColor &color, Vec2i v1, Vec2i v2) {
    int slope_error_prime = 0;
    // If the line is steep, then we swap x with y
    bool steep = false;
    if (std::abs(v2.y - v1.y) > std::abs(v2.x - v1.x)) {
        std::swap(v1.x, v1.y);
        std::swap(v2.x, v2.y);
        steep = true;
    }
    if (v1.x > v2.x) {
        std::swap(v1.x, v2.x);
        std::swap(v1.y, v2.y);
    }
    int y = v1.y;
    for (int x = v1.x; x <= v2.x; x++) {
        if (steep) {
            image->set(y, x, color);
        } else {
            image->set(x, y, color);
        }
        int x_delta = v2.x - v1.x;
        int y_delta = v2.y - v1.y;
        int temp_error = 2 * (slope_error_prime + y_delta);
        // Case for slope is negative
        if (v2.y <= v1.y) {
            if (temp_error > -x_delta) {
                slope_error_prime += y_delta;
            } else {
                y--;
                slope_error_prime += y_delta + x_delta;
            }
        } else {
            if (temp_error < x_delta) {
                slope_error_prime += y_delta;
            } else {
                y++;
                slope_error_prime += y_delta - x_delta;
            }
        }
    }
}

Vec3f barycentric(Vec3f *pts, const Vec3f &p) {
    // Cross product
    Vec3f u = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - p.x) ^
              Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - p.y);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u.z)` < 1 means `u.z` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(u.z) < 1) return {-1, 1, 1};
    return {1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

void drawTriangle(TGAImage *image, Model *model, float light_intensity, Vec3f *pts, Vec2f *texture_pts,
                  float *depth_buffer) {
    /* Iterate all pixels in a bounding box that contains the triangle
     * and check if the pixel is within the triangle using the
     * barycentric coordinates
    */
    int x_max = 0;
    int x_min = image->get_width() - 1;
    int y_max = 0;
    int y_min = image->get_height() - 1;
    // Iterate the points to find the smallest bounding box that fits the triangle
    for (int i = 0; i < 3; i++) {
        if (pts[i].x > static_cast<float>(x_max)) x_max = pts[i].x;
        if (pts[i].x < static_cast<float>(x_min)) x_min = pts[i].x;
        if (pts[i].y > static_cast<float>(y_max)) y_max = pts[i].y;
        if (pts[i].y < static_cast<float>(y_min)) y_min = pts[i].y;
    }
    Vec3f p;
    for (int x = x_min; x <= x_max; x++) {
        for (int y = y_min; y <= y_max; y++) {
            p.x = static_cast<float>(x);
            p.y = static_cast<float>(y);
            p.z = 0;

            Vec3f b_coord = barycentric(pts, p);
            Vec2f texture_coord =
                    texture_pts[0] * b_coord[0] + texture_pts[1] * b_coord[1] + texture_pts[2] * b_coord[2];
            if (b_coord.x < 0 || b_coord.y < 0 || b_coord.z < 0) continue;
            // The z coord of the pixel on the triangle is the barycentric coord of the point dot with
            // the z coords of the three points of the triangle
            for (int i = 0; i < 3; i++) {
                p.z += pts[i].z * b_coord[i];
            }
            TGAColor color = model->diffuse(texture_coord);
            if (depth_buffer[static_cast<int>(p.x + p.y * static_cast<float>(image->get_width()))] < p.z) {
                depth_buffer[static_cast<int>(p.x + p.y * static_cast<float>(image->get_width()))] = p.z;
                image->set(p.x, p.y, color * light_intensity);
            }
        }
    }
}

Matrix camera_matrix(const Vec3f &eye, const Vec3f &center, const Vec3f &up) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = (up ^ z).normalize();
    Vec3f y = (z ^ x).normalize();
    Matrix model_view = Matrix::identity(4);
    Matrix translation = Matrix::identity(4);
    for (int i = 0; i < 3; i++) {
        model_view[0][i] = x[i];
        model_view[1][i] = y[i];
        model_view[2][i] = z[i];
        translation[i][3] = -center[i];
    }
    return model_view * translation;
}

Matrix projection_matrix(float camera_distance) {
    // Create a 4 by 4 matrix
    Matrix mat = Matrix::identity(4);
    mat[3][2] = -1.f / camera_distance;
    return mat;
}

Vec3f m2v(const Matrix &m) {
    return {m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]};
}

Matrix v2m(const Vec3f &v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

Matrix viewport(int x, int y, int w, int h, int depth) {
    Matrix m = Matrix::identity(4);
    m[0][3] = static_cast<float>(x) + static_cast<float>(w) / 2.f;
    m[1][3] = static_cast<float>(y) + static_cast<float>(h) / 2.f;
    m[2][3] = static_cast<float>(depth) / 2.f;

    m[0][0] = static_cast<float>(w) / 2.f;
    m[1][1] = static_cast<float>(h) / 2.f;
    m[2][2] = static_cast<float>(depth) / 2.f;
    return m;
}


void drawFace(TGAImage *image, Matrix *p_m, Matrix *v_p, Matrix*c_m, Model *model, const Vec3f &light_dir, int width, int height) {
    // Depth buffer
    float depth_buffer[width * height];
    for (int i = 0; i < width * height; i++) {
        depth_buffer[i] = std::numeric_limits<float>::lowest();
    }
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<Vec2i> face = model->face(i);
        Vec3f world_coords[3];
        Vec2f texture_coords[3];
        Vec3f world2screen_coords[3];
        for (int j = 0; j < 3; j++) {
            world_coords[j] = model->vert(face[j][0]);
            texture_coords[j] = model->texture_vert(face[j][1]);
            world2screen_coords[j] = m2v(*v_p * *p_m * *c_m * v2m(world_coords[j]));
        }
        // The normal vector of a triangle is just cross product of two sides
        Vec3f normal = (world_coords[2] - world_coords[0]) ^(world_coords[1] - world_coords[0]);
        normal.normalize();
        float light_intensity = normal * light_dir;
        if (light_intensity > 0) {
            drawTriangle(image, model, light_intensity, world2screen_coords, texture_coords, depth_buffer);
        }
    }
}

int main(int argc, char **argv) {
    const int width = 512;
    const int height = 512;
    const int screen_depth = 255;
    float camera_distance_on_z = 3.f;

    TGAImage image(width, height, TGAImage::RGB);
    std::unique_ptr<Model> model(new Model("obj/african_head.obj"));

    // View transformation setup
    Vec3f light_source(0, 0, -1);
    Vec3f eye(1, 1, 3);
    Vec3f up(0, 1, 0);
    Vec3f model_center(0, 0, 0);
    Matrix p_matrix = projection_matrix(camera_distance_on_z);
    Matrix v_port = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, screen_depth);
    Matrix c_matrix = camera_matrix(eye, model_center, up);
    drawFace(&image, &p_matrix, &v_port, &c_matrix, model.get(), light_source, width, height);

    // Origin at the left bottom corner of the image
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
