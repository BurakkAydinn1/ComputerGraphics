#include "mesh.hpp"

void Mesh::addTriangle(const Triangle &tri)
{
    triangles.push_back(tri);
}

Intersection Mesh::intersect(const Ray &ray, float tMin, float tMax) const
{
    Intersection closestHit = {false, tMax};

    for (const Triangle &tri : triangles)
    {
        Intersection hit = tri.intersect(ray, tMin, closestHit.t);
        if (hit.hit && hit.t < closestHit.t)
        {
            closestHit = hit;
        }
    }

    return closestHit;
}
