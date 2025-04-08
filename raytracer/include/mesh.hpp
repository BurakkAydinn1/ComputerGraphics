#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "triangle.hpp"

class Mesh
{
public:
    std::vector<Triangle> triangles;

    void addTriangle(const Triangle &tri);
    Intersection intersect(const Ray &ray, float tMin = 0.001f, float tMax = 1e9f) const;
};

#endif
