//
// Created by peter on 2020-03-19.
//

#ifndef TINYRENDERER_SHADER_H
#define TINYRENDERER_SHADER_H

#include <algorithm>
#include <cmath>

#include "geometry/geometry.h"
#include "loader/tgaimage.h"

class Shader {
private:
    float ambient_lighting_;
    float diffuse_reflectivity_;
    float specular_reflectivity_;
    float specular_intensity_;
    Vec3f light_dir_;
public:
    Shader(float ambient_lighting, float diffuse_reflectivity, float specular_reflectivity, float specular_intensity, const Vec3f &light_dir);
    TGAColor get_shaded_color(const TGAColor &color, const Vec3f &normal, const Vec3f &viewing_location, const Vec3f &current_pt) const;
};


#endif //TINYRENDERER_SHADER_H