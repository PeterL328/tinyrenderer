//
// Created by peter on 2020-03-12.
//
#ifndef TINYRENDERER_TINY_GL_H
#define TINYRENDERER_TINY_GL_H

#include <cmath>
#include <limits>

#include "geometry/matrix.h"
#include "geometry/vector.h"
#include "loader/tgaimage.h"
#include "tiny_gl.h"
#include "model.h"
#include "shader.h"


namespace tiny_gl {
    void drawLine(TGAImage *image, const TGAColor &color, Geometry::Vec2i v1, Geometry::Vec2i v2);

    void
    drawTriangle(TGAImage *image, Model *model, const Shader &shader, const Geometry::Vec3f *pts, const Geometry::Vec2f *texture_pts,
                 const Geometry::Vec3f *normal_pts, float *depth_buffer, const Geometry::Vec3f &camera_source);

    void drawFace(TGAImage *image, const Geometry::Matrix &p_m, const Geometry::Matrix &v_p, const Geometry::Matrix &c_m, Model *model,
                  const Shader &shader, const Geometry::Vec3f &camera_source, const int width, const int height);

    Geometry::Matrix camera_matrix(const Geometry::Vec3f &eye, const Geometry::Vec3f &center, const Geometry::Vec3f &up);

    Geometry::Matrix projection_matrix(const float camera_distance);

    Geometry::Vec3f hom2cart(const Geometry::Matrix &m);

    Geometry::Matrix cart2hom(const Geometry::Vec3f &v);

    Geometry:: Matrix viewport(const int x, const int y, const int w, const int h, const int depth);

    Geometry::Vec3f barycentric(const Geometry::Vec3f *pts, const Geometry::Vec3f &p);
}

#endif //TINYRENDERER_TINY_GL_H
