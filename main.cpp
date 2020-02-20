#include <cmath>
#include <memory>

#include "tgaimage.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void drawLine(TGAImage *image, const TGAColor &color, int x1, int x2, int y1, int y2) {
    int slope_error_prime = 0;
    // If the line is steep, then we swap x with y
    bool steep = false;
    if (std::abs(y2 - y1) > std::abs(x2 - x1)) {
        std::swap(x1, y1);
        std::swap(x2, y2);
        steep = true;
    }
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    int y = y1;
    for (int x = x1; x <= x2; x++) {
        if (steep) {
            image->set(y, x, color);
        } else {
            image->set(x, y, color);
        }
        int x_delta = x2 - x1;
        int y_delta = y2 - y1;
        int temp_error = 2 * (slope_error_prime + y_delta);
        // Case for slope is negative
        if (y2 <= y1) {
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

void drawFace(TGAImage *image, Model *model, int width, int height) {
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x1 = static_cast<int>((v0.x + 1.) * width / 2.);
            int y1 = static_cast<int>((v0.y + 1.) * height / 2.);
            int x2 = static_cast<int>((v1.x + 1.) * width / 2.);
            int y2 = static_cast<int>((v1.y + 1.) * height / 2.);
            drawLine(image, white, x1, x2, y1, y2);
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
