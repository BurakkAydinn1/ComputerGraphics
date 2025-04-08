#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "material.hpp"

struct Intersection
{
    bool hit;
    float t;
    Vec3 point;
    Vec3 normal;
    int materialIndex;
};

class Triangle
{
public:
    Vec3 v0, v1, v2;
    Vec3 normal;
    int materialIndex;

    Triangle();
    Triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c, int materialIdx);

    Intersection intersect(const Ray &ray, float tMin = 0.001f, float tMax = 1e9f) const;
};

#endif
