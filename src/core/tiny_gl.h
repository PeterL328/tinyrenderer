//
// Created by peter on 2020-03-12.
//
#ifndef TINYRENDERER_TINY_GL_H
#define TINYRENDERER_TINY_GL_H

#include <cmath>
#include <limits>

#include "geometry/geometry.h"
#include "loader/tgaimage.h"
#include "tiny_gl.h"
#include "model.h"
#include "shader.h"

namespace tiny_gl {
    void drawLine(TGAImage *image, const TGAColor &color, Vec2i v1, Vec2i v2);

    void
    drawTriangle(TGAImage *image, Model *model, const Shader &shader, const Vec3f *pts, const Vec2f *texture_pts,
                 const Vec3f *normal_pts, float *depth_buffer, const Vec3f &camera_source);

    void drawFace(TGAImage *image, const Matrix &p_m, const Matrix &v_p, const Matrix &c_m, Model *model,
                  const Shader &shader, const Vec3f &camera_source, const int width, const int height);

    Matrix camera_matrix(const Vec3f &eye, const Vec3f &center, const Vec3f &up);

    Matrix projection_matrix(const float camera_distance);

    Vec3f hom2cart(const Matrix &m);

    Matrix cart2hom(const Vec3f &v);

    Matrix viewport(const int x, const int y, const int w, const int h, const int depth);

    Vec3f barycentric(const Vec3f *pts, const Vec3f &p);
}

#endif //TINYRENDERER_TINY_GL_H
