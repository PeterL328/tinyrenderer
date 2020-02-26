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

Vec3f barycentric(Vec3f *pts, Vec3f p) {
    // Cross product
    Vec3f u = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - p.x) ^
              Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - p.y);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u.z)` < 1 means `u.z` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(u.z) < 1) return {-1, 1, 1};
    return {1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

void drawTriangle(TGAImage *image, Model *model, float light_intensity, Vec3f *pts, Vec2f *texture_pts, float *depth_buffer) {
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
            Vec2f texture_coord = texture_pts[0] * b_coord[0] + texture_pts[1] * b_coord[1] + texture_pts[2] * b_coord[2];
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

Vec3f world2screen(Vec3f v, int screen_width, int screen_height) {
    return {
            static_cast<float>(lround((v.x + 1.) * screen_width / 2. + .5)),
            static_cast<float>(lround((v.y + 1.) * screen_height / 2. + .5)),
            v.z
    };
}

void drawFace(TGAImage *image, Model *model, int width, int height) {
    // Light source
    Vec3f light_dir(0, 0, -1);
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
            world2screen_coords[j] = world2screen(world_coords[j], width, height);
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
    int width = 1024;
    int height = 1024;

    TGAImage image(width, height, TGAImage::RGB);
    std::unique_ptr<Model> model(new Model("obj/african_head.obj"));
    drawFace(&image, model.get(), width, height);

    // Origin at the left bottom corner of the image
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
