#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "vec3.hpp"

struct AmbientLight {
    Vec3 intensity;
};

struct PointLight {
    Vec3 position;
    Vec3 intensity;
};

struct TriangularLight {
    Vec3 v0, v1, v2;
    Vec3 intensity;
};

#endif
