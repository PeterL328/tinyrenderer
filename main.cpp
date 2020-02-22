#include <cmath>
#include <memory>

#include "geometry.h"
#include "tgaimage.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 10);


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

Vec3f barycentric(Vec2i *pts, Vec2i p) {
    // Cross product
    Vec3f u = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - p.x) ^
              Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - p.y);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u.z)` < 1 means `u.z` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(u.z) < 1) return {-1, 1, 1};
    return {1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

void drawTriangle(TGAImage *image, const TGAColor &color, Vec2i *pts) {
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
        if (pts[i].x > x_max) x_max = pts[i].x;
        if (pts[i].x < x_min) x_min = pts[i].x;
        if (pts[i].y > y_max) y_max = pts[i].y;
        if (pts[i].y < y_min) y_min = pts[i].y;
    }
    for (int x = x_min; x <= x_max; x++) {
        for (int y = y_min; y <= y_max; y++) {
            Vec3f b_check = barycentric(pts, Vec2i(x, y));
            if (b_check.x < 0 || b_check.y < 0 || b_check.z < 0) continue;
            image->set(x, y, color);
        }
    }
}

void drawFace(TGAImage *image, Model *model, int width, int height) {
    Vec3f light_dir(0, 0, -1);
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++) {
            Vec3f point = model->vert(face[j]);
            screen_coords[j] = Vec2i(
                    static_cast<int>((point.x + 1.) * width / 2.),
                    static_cast<int>((point.y + 1.) * height / 2.)
            );
            world_coords[j] = point;
        }
        // The normal vector of a triangle is just cross product of two sides
        Vec3f normal = (world_coords[2] - world_coords[0]) ^(world_coords[1] - world_coords[0]);
        normal.normalize();
        float light_intensity = normal * light_dir;
        if (light_intensity > 0) {
            drawTriangle(image, TGAColor(light_intensity * 255, light_intensity * 255, light_intensity * 255, 255),
                         screen_coords);
        }
    }
}

int main(int argc, char **argv) {
    int height = 512;
    int width = 512;
    TGAImage image(width, height, TGAImage::RGB);
    std::unique_ptr<Model> model(new Model("obj/african_head.obj"));
    drawFace(&image, model.get(), width, height);

    // Origin at the left bottom corner of the image
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
