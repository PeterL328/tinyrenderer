cmake_minimum_required(VERSION 3.15)
project(tinyrenderer)

set(CMAKE_CXX_STANDARD 14)


file(GLOB_RECURSE TR_SOURCES "src/*.cpp")
file(GLOB_RECURSE TR_HEADERS "src/*.h")

# Iterate to find all source files
set (TR_SRC_DIRS "src/")
foreach (_headerFile ${TR_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND TR_SRC_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES TR_SRC_DIRS)

add_executable (tinyrenderer ${TR_SOURCES})
target_include_directories(tinyrenderer PRIVATE ${TR_SRC_DIRS})

# Required packages/libraries
find_package(CLI11 REQUIRED)
target_link_libraries(tinyrenderer CLI11::CLI11)