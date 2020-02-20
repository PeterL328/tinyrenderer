#include "tgaimage.h"
#include <cmath>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void drawLine(TGAImage *image, const TGAColor& color, int x1, int x2, int y1, int y2) {
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

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    drawLine(&image, white, 99, 0, 0, 99);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
