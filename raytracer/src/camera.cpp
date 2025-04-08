#include "camera.hpp"

Camera::Camera() {}

void Camera::initialize()
{
    // Gaze ve up normalize edilmeli
    Vec3 gazeNorm = gaze.normalize();
    Vec3 upNorm = up.normalize();

    w = (gazeNorm * -1).normalize(); // Kamera arkaya bakıyor
    u = upNorm.cross(w).normalize();
    v = w.cross(u).normalize();

    float imagePlaneWidth = right - left;
    float imagePlaneHeight = top - bottom;

    Vec3 center = position + gazeNorm * nearDistance;
    topLeftCorner = center + u * left + v * top;

    horizontalStep = u * (imagePlaneWidth / imageWidth);
    verticalStep = v * (-imagePlaneHeight / imageHeight); // - çünkü y aşağı akar
}

Ray Camera::generateRay(int pixelX, int pixelY) const
{
    Vec3 pixelCenter = topLeftCorner + horizontalStep * (pixelX + 0.5f) + verticalStep * (pixelY + 0.5f);
    Vec3 direction = (pixelCenter - position).normalize();
    return Ray(position, direction);
}
