//
// Created by peter on 2020-02-20.
//

#ifndef TINYRENDERER_MODEL_H
#define TINYRENDERER_MODEL_H

#include <vector>
#include <string>

#include "geometry/vector.h"
#include "loader/tgaimage.h"


class Model {
private:
    std::vector<Geometry::Vec3f> verts_;
    std::vector<Geometry::Vec3f> norms_;
    std::vector<Geometry::Vec2f> texture_verts_;
    std::vector<std::vector<Geometry::Vec3i> > faces_;

    TGAImage diffusemap_;
public:
    explicit Model(const std::string &filename);

    ~Model();

    int nverts();

    int ntexture_verts();

    int nfaces();

    Geometry::Vec3f vert(int i);

    Geometry::Vec3f vert(int iface, int nthvert);

    Geometry:: Vec2f texture_vert(int i);

    Geometry::Vec2f texture_vert(int iface, int nthvert);

    Geometry::Vec3f normal(int i);

    Geometry::Vec3f normal(int iface, int nthvert);

    std::vector<Geometry::Vec3i> face(int idx);

    TGAColor diffuse(const Geometry::Vec2f &uvf);

    static void load_texture(const std::string &filename, const char *suffix, TGAImage &image);
};

#endif //TINYRENDERER_MODEL_H