#include <memory>

#include <CLI/CLI.hpp>

#include "geometry/geometry.h"
#include "loader/tgaimage.h"
#include "core/tiny_gl.h"
#include "model/model.h"


int main(int argc, char **argv) {
    // Parse arguments
    CLI::App app{"Tiny rendering engine"};

    int width = 512;
    int height = 512;
    const int screen_depth = 255;
    const float camera_distance_on_z = 3.f;
    std::string object_file_path = "obj/african_head.obj";
    std::string output_file_path = "output.tga";

    app.add_option("--width", width, "The width of the final rendering");
    app.add_option("--height", height, "The height of the final rendering");
    app.add_option("--obj", object_file_path, "The path to the object file for rendering");
    app.add_option("--output,-o", output_file_path, "The path to the output rendering file");

    CLI11_PARSE(app, argc, argv);

    TGAImage image(width, height, TGAImage::RGB);
    std::unique_ptr<Model> model(new Model(object_file_path));

    // View transformation setup
    Vec3f light_source(0, 0, -1);
    Vec3f eye(1, 1, 3);
    Vec3f up(0, 1, 0);
    Vec3f model_center(0, 0, 0);
    Matrix p_matrix = tiny_gl::projection_matrix(camera_distance_on_z);
    Matrix v_port = tiny_gl::viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, screen_depth);
    Matrix c_matrix = tiny_gl::camera_matrix(eye, model_center, up);
    tiny_gl::drawFace(&image, &p_matrix, &v_port, &c_matrix, model.get(), light_source, width, height);

    // Origin at the left bottom corner of the image
    image.flip_vertically();
    image.write_tga_file(output_file_path);
    return 0;
}
