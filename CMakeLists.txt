cmake_minimum_required(VERSION 3.15)
project(tinyrenderer)

set(CMAKE_CXX_STANDARD 14)
# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


file(GLOB_RECURSE TR_SOURCES "src/*.cpp")
file(GLOB_RECURSE TR_HEADERS "src/*.h")
file(GLOB_RECURSE TR_VIEWS "src/gui/view/*.ui")

# Iterate to find all source files
set (TR_SRC_DIRS "src/")
foreach (_headerFile ${TR_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND TR_SRC_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES TR_SRC_DIRS)

add_executable (tinyrenderer ${TR_SOURCES} ${TR_VIEWS})
target_include_directories(tinyrenderer PRIVATE ${TR_SRC_DIRS})

# Required packages/libraries
find_package(CLI11 REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(tinyrenderer CLI11::CLI11 Qt5::Widgets)