#include <memory>

#include <CLI/CLI.hpp>
#include <QApplication>

#include "core/tiny_gl.h"
#include "core/model.h"
#include "core/shader.h"
#include "geometry/matrix.h"
#include "geometry/vector.h"
#include "gui/mainwindow.h"


int main(int argc, char **argv) {
    // Parse arguments
    CLI::App app{"Tiny rendering engine"};

    int width = 512;
    int height = 512;
    const int screen_depth = 255;
    const float camera_distance_on_z = 3.f;
    std::string object_file_path = "examples/african_head.obj";
    std::string output_file_path = "output.tga";

    app.add_option("--width", width, "The width of the final rendering");
    app.add_option("--height", height, "The height of the final rendering");
    app.add_option("--obj", object_file_path, "The path to the object file for rendering");
    app.add_option("--output,-o", output_file_path, "The path to the output rendering file");

    CLI11_PARSE(app, argc, argv);

    // Setup Qt
    QApplication qt_app(argc, argv);
    MainWindow window{width, height};

    std::unique_ptr<Model> model(new Model(object_file_path));

    // View transformation setup
    Geometry::Vec3f light_source(1500, 1500, 1500);
    Geometry::Vec3f eye(-200, -200, 500);
    Geometry::Vec3f up(0, 1, 0);
    Geometry::Vec3f model_center(0, 0, 0);
    Geometry::Matrix p_matrix = TinyGL::projection_matrix(camera_distance_on_z);
    Geometry::Matrix v_port = TinyGL::viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, screen_depth);
    Geometry::Matrix c_matrix = TinyGL::camera_matrix(eye, model_center, up);

    // Shader setup
    Shader shader{0.2f, 0.6f, 0.2f, 10, light_source};
    TinyGL::drawFace(&window, p_matrix, v_port, c_matrix, model.get(), shader, eye, width, height);

    // Display application window
    window.show();
    window.display();
    return qt_app.exec();
}
