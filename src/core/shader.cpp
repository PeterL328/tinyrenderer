//
// Created by peter on 2020-03-19.
//

#include "shader.h"


Shader::Shader(const float ambient_lighting, const float diffuse_reflectivity, const float specular_reflectivity, const float specular_intensity, const Geometry::Vec3f &light_dir):
    ambient_lighting_(ambient_lighting), diffuse_reflectivity_(diffuse_reflectivity), specular_reflectivity_(specular_reflectivity), specular_intensity_(specular_intensity), light_dir_(light_dir){}


TGAColor Shader::get_shaded_color(const TGAColor &color, const Geometry::Vec3f &normal, const Geometry::Vec3f &viewing_location, const Geometry::Vec3f &current_pt) const {
    TGAColor shaded_color{};
    Geometry::Vec3f norm_point_to_light = (light_dir_ * -1).normalize();
    float light_n_normal = std::max(0.f, normal * norm_point_to_light);
    Geometry::Vec3f norm_point_to_camera = (viewing_location - current_pt).normalize();
    Geometry::Vec3f h = (norm_point_to_camera + norm_point_to_light).normalize();
    float specular_term = std::max(0.f, specular_reflectivity_ * powf(normal * h, specular_intensity_));
    shaded_color.r = std::min(255, static_cast<int>(color.r * (ambient_lighting_ + (diffuse_reflectivity_ * light_n_normal)) + specular_term));
    shaded_color.g = std::min(255, static_cast<int>(color.g * (ambient_lighting_ + (diffuse_reflectivity_ * light_n_normal)) + specular_term));
    shaded_color.b = std::min(255, static_cast<int>(color.b * (ambient_lighting_ + (diffuse_reflectivity_ * light_n_normal)) + specular_term));
    return shaded_color;
}


