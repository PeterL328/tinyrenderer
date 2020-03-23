//
// Created by peter on 2020-03-19.
//

#include "shader.h"


Shader::Shader(const float ambient_lighting, const float diffuse_reflectivity, const float specular_reflectivity,
               const float specular_intensity, const Geometry::Vec3f &light_source) :
        ambient_lighting_(ambient_lighting), diffuse_reflectivity_(diffuse_reflectivity),
        specular_reflectivity_(specular_reflectivity), specular_intensity_(specular_intensity),
        light_source_(light_source), specular_color_ (255, 255, 255, 0) {}


Shader::Shader(float ambient_lighting, float diffuse_reflectivity, float specular_reflectivity,
               float specular_intensity, const Geometry::Vec3f &light_source, const TGAColor &specular_color) :
        ambient_lighting_(ambient_lighting), diffuse_reflectivity_(diffuse_reflectivity),
        specular_reflectivity_(specular_reflectivity), specular_intensity_(specular_intensity),
        light_source_(light_source), specular_color_ (specular_color) {}


TGAColor
Shader::get_shaded_color(const TGAColor &color, const Geometry::Vec3f &normal, const Geometry::Vec3f &viewing_location,
                         const Geometry::Vec3f &current_pt) const {
    TGAColor shaded_color{};
    Geometry::Vec3f norm_point_to_light = (light_source_ - current_pt).normalize();
    float light_n_normal = std::max(0.f, normal * norm_point_to_light);
    float ambient_n_diffuse_term = ambient_lighting_ + (diffuse_reflectivity_ * light_n_normal);

    Geometry::Vec3f norm_point_to_camera = (viewing_location - current_pt).normalize();
    Geometry::Vec3f h = (norm_point_to_camera + norm_point_to_light).normalize();
    float specular_power_term =  powf(normal * h, specular_intensity_);
    shaded_color.r = std::min(255, static_cast<int>(color.r * ambient_n_diffuse_term + std::max(0.f, specular_color_.r * specular_reflectivity_ * specular_power_term)));
    shaded_color.g = std::min(255, static_cast<int>(color.g * ambient_n_diffuse_term + std::max(0.f, specular_color_.g * specular_reflectivity_ * specular_power_term)));
    shaded_color.b = std::min(255, static_cast<int>(color.b * ambient_n_diffuse_term + std::max(0.f, specular_color_.b * specular_reflectivity_ * specular_power_term)));
    return shaded_color;
}


