#include "ray.hpp"

Ray::Ray() : origin(Vec3()), direction(Vec3(0, 0, -1)) {}

Ray::Ray(const Vec3 &origin, const Vec3 &direction)
    : origin(origin), direction(direction.normalize()) {}

Vec3 Ray::at(float t) const
{
    return origin + direction * t;
}
