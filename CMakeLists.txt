cmake_minimum_required(VERSION 3.15)
project(tinyrenderer)

set(CMAKE_CXX_STANDARD 14)

# Required packages/libraries
find_package(CLI11 REQUIRED)

add_executable(tinyrenderer main.cpp tgaimage.cpp tgaimage.h geometry.h model.cpp model.h geometry.cpp tiny_gl.cpp tiny_gl.h)
target_link_libraries(tinyrenderer CLI11::CLI11)
