#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "vec3.hpp"

struct Material
{
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
    Vec3 mirror;
    float phongExponent;
    float textureFactor;
};

#endif
