# Tiny Renderer Engine
This 3D graphics engine is able to render a .obj file on screen and
does is built from scratch not using frameworks like OpenGL.

## Installation Guide
To run this program, we need to install QT5 [link](https://www.qt.io/download) and CLI11 [link](https://github.com/CLIUtils/CLI11).

```shell
git clone git@github.com:PeterL328/tinyrenderer.git
cd tinyrenderer
mkdir build
cd build
cmake ..
make
./tinyrenderer ../example/african_head.obj 
```