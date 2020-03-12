//
// Created by peter on 2020-02-20.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const std::string &filename) : verts_(), texture_verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line);
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (float &i : v.raw) iss >> i;
            verts_.push_back(v);
        } else if (!line.compare(0, 4, "vt  ")) {
            iss >> trash >> trash;
            Vec2f v;
            for (float &i : v.raw) iss >> i;
            texture_verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec2i> f;
            int itrash = 0;
            Vec2i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> itrash) {
                for (int &i : tmp.raw) i--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
}

Model::~Model() = default;

int Model::nverts() {
    return static_cast<int> (verts_.size());
}

int Model::nfaces() {
    return static_cast<int>(faces_.size());
}

int Model::ntexture_verts() {
    return static_cast<int>(texture_verts_.size());;
}

std::vector<Vec2i> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2f Model::texture_vert(int i) {
    return texture_verts_[i];
}

void Model::load_texture(const std::string &filename, const char *suffix, TGAImage &image) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of('.');
    if (dot != std::string::npos) {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading "
                  << (image.read_tga_file(texfile.c_str()) ? "ok" : "failed")
                  << std::endl;
        image.flip_vertically();
    }
}

TGAColor Model::diffuse(const Vec2f &uvf) {
    Vec2i uv(
            uvf.x * static_cast<float>(diffusemap_.get_width()),
            uvf.y * static_cast<float>(diffusemap_.get_height())
    );
    return diffusemap_.get(uv.x, uv.y);
}

