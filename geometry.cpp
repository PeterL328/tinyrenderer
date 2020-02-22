//
// Created by peter on 2020-02-22.
//
#include "geometry.h"

Vec3f cross(Vec3f p0, Vec3f p1) {
    return {(p0.y * p1.z) - (p0.z * p1.y), (p0.z * p1.x) - (p0.x * p1.z), (p0.x * p1.y) - (p0.y * p1.x)};
}