#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vec3.hpp"
#include "ray.hpp"

class Camera
{
public:
    Vec3 position;
    Vec3 gaze;
    Vec3 up;
    float left, right, bottom, top;
    float nearDistance;
    int imageWidth, imageHeight;

    Vec3 u, v, w; // kamera eksen sistemi
    Vec3 topLeftCorner;
    Vec3 horizontalStep;
    Vec3 verticalStep;

    Camera();
    void initialize();

    Ray generateRay(int pixelX, int pixelY) const;
};

#endif
