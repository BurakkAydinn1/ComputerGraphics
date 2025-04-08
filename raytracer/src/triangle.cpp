#include "triangle.hpp"

Triangle::Triangle() {}

Triangle::Triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c, int materialIdx)
    : v0(a), v1(b), v2(c), materialIndex(materialIdx)
{
    normal = (v1 - v0).cross(v2 - v0).normalize();
}

// Function to check if a ray intersects with the triangle
// Returns an Intersection object containing the intersection details
// If there is no intersection, it returns an Intersection object with isIntersected set to false
// The function uses the Möller–Trumbore intersection algorithm
// Reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_algorithm

Intersection Triangle::intersect(const Ray &ray, float tMin, float tMax) const
{
    const float EPSILON = 1e-6f;
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    Vec3 h = ray.direction.cross(edge2);
    float a = edge1.dot(h);

    if (fabs(a) < EPSILON)
        return {false}; // Ray paralel

    float f = 1.0f / a;
    Vec3 s = ray.origin - v0;
    float u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f)
        return {false};

    Vec3 q = s.cross(edge1);
    float v = f * ray.direction.dot(q);
    if (v < 0.0f || u + v > 1.0f)
        return {false};

    float t = f * edge2.dot(q);
    if (t < tMin || t > tMax)
        return {false};

    Vec3 hitPoint = ray.at(t);
    return {true, t, hitPoint, normal, materialIndex};
}
