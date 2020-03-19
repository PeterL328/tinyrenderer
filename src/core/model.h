//
// Created by peter on 2020-02-20.
//

#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>

#include "geometry/geometry.h"
#include "loader/tgaimage.h"

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<Vec3f> norms_;
    std::vector<Vec2f> texture_verts_;
    std::vector<std::vector<Vec3i> > faces_;

    TGAImage diffusemap_;
public:
    explicit Model(const std::string &filename);

    ~Model();

    int nverts();

    int ntexture_verts();

    int nfaces();

    Vec3f vert(int i);

    Vec3f vert(int iface, int nthvert);

    Vec2f texture_vert(int i);

    Vec2f texture_vert(int iface, int nthvert);

    Vec3f normal(int i);

    Vec3f normal(int iface, int nthvert);

    std::vector<Vec3i> face(int idx);

    TGAColor diffuse(const Vec2f &uvf);

    static void load_texture(const std::string &filename, const char *suffix, TGAImage &image);
};

#endif //__MODEL_H__