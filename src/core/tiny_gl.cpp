//
// Created by peter on 2020-03-12.
//

#include "tiny_gl.h"


using namespace Geometry;

namespace TinyGL {
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

    void
    drawTriangle(MainWindow *window, Model *model, const Shader &shader, const Vec3f *pts, const Vec2f *texture_pts,
                 const Vec3f *normal_pts, float *depth_buffer, const Vec3f &camera_source) {
        /* Iterate all pixels in a bounding box that contains the triangle
         * and check if the pixel is within the triangle using the
         * barycentric coordinates
        */
        int x_max = 0;
        int x_min = window->get_width() - 1;
        int y_max = 0;
        int y_min = window->get_height() - 1;
        // Iterate the points to find the smallest bounding box that fits the triangle
        for (int i = 0; i < 3; i++) {
            if (pts[i].x > static_cast<float>(x_max)) x_max = pts[i].x;
            if (pts[i].x < static_cast<float>(x_min)) x_min = pts[i].x;
            if (pts[i].y > static_cast<float>(y_max)) y_max = pts[i].y;
            if (pts[i].y < static_cast<float>(y_min)) y_min = pts[i].y;
        }
        Vec3f p{};
        for (int x = x_min; x <= x_max; x++) {
            for (int y = y_min; y <= y_max; y++) {
                p.x = static_cast<float>(x);
                p.y = static_cast<float>(y);
                p.z = 0;

                Vec3f b_coord = barycentric(pts, p);
                if (b_coord.x < 0 || b_coord.y < 0 || b_coord.z < 0) continue;
                Vec2f texture_coord{};
                Vec3f normal{};
                // The z coord of the pixel on the triangle is the barycentric coord of the point dot with
                // the z coords of the three points of the triangle
                for (int i = 0; i < 3; i++) {
                    p.z += pts[i].z * b_coord[i];
                    texture_coord += texture_pts[i] * b_coord[i];
                    normal += normal_pts[i] * b_coord[i];
                }
                TGAColor tg_color = model->diffuse(texture_coord);
                QColor color = {tg_color.r, tg_color.g, tg_color.b};
                if (depth_buffer[static_cast<int>(p.x + p.y * static_cast<float>(window->get_width()))] < p.z) {
                    depth_buffer[static_cast<int>(p.x + p.y * static_cast<float>(window->get_height()))] = p.z;
                    QColor shaded_color = shader.get_shaded_color(color, normal, camera_source, p);
                    window->set_pixel(p.x, p.y, shaded_color);
                }
            }
        }
    }

    void drawFace(MainWindow *window, const Matrix &p_m, const Matrix &v_p, const Matrix &c_m, Model *model,
                  const Shader &shader, const Vec3f &camera_source, const int width, const int height) {
        // Depth buffer
        float depth_buffer[width * height];
        for (int i = 0; i < width * height; i++) {
            depth_buffer[i] = std::numeric_limits<float>::lowest();
        }
        for (int i = 0; i < model->nfaces(); i++) {
            Vec3f world_coords[3];
            Vec3f normal_coords[3];
            Vec2f texture_coords[3];
            Vec3f world2screen_coords[3];
            for (int j = 0; j < 3; j++) {
                world_coords[j] = model->vert(i, j);
                texture_coords[j] = model->texture_vert(i, j);
                normal_coords[j] = model->normal(i, j);
                world2screen_coords[j] = hom2cart(v_p * p_m * c_m * cart2hom(world_coords[j]));
            }
            drawTriangle(window, model, shader, world2screen_coords, texture_coords, normal_coords, depth_buffer,
                         camera_source);
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

    Matrix projection_matrix(const float camera_distance) {
        // Create a 4 by 4 matrix
        Matrix mat = Matrix::identity(4);
        mat[3][2] = -1.f / camera_distance;
        return mat;
    }

    Vec3f hom2cart(const Matrix &m) {
        return {m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]};
    }

    Matrix cart2hom(const Vec3f &v) {
        Matrix m(4, 1);
        m[0][0] = v.x;
        m[1][0] = v.y;
        m[2][0] = v.z;
        m[3][0] = 1.f;
        return m;
    }

    Matrix viewport(const int x, const int y, const int w, const int h, const int depth) {
        Matrix m = Matrix::identity(4);
        m[0][3] = static_cast<float>(x) + static_cast<float>(w) / 2.f;
        m[1][3] = static_cast<float>(y) + static_cast<float>(h) / 2.f;
        m[2][3] = static_cast<float>(depth) / 2.f;

        m[0][0] = static_cast<float>(w) / 2.f;
        m[1][1] = static_cast<float>(h) / 2.f;
        m[2][2] = static_cast<float>(depth) / 2.f;
        return m;
    }

    Vec3f barycentric(const Vec3f *pts, const Vec3f &p) {
        // Cross product
        Vec3f u = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - p.x) ^
                  Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - p.y);
        /* `pts` and `P` has integer value as coordinates
           so `abs(u.z)` < 1 means `u.z` is 0, that means
           triangle is degenerate, in this case return something with negative coordinates */
        if (std::abs(u.z) < 1) return {-1, 1, 1};
        return {1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
    }
}