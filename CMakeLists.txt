cmake_minimum_required(VERSION 3.15)
project(tinyrenderer)

set(CMAKE_CXX_STANDARD 14)

add_executable(tinyrenderer main.cpp tgaimage.cpp tgaimage.h geometry.h geometry.cpp model.cpp model.h)