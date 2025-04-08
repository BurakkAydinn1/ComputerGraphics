#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "camera.hpp"
#include "vec3.hpp"
#include "light.hpp"
#include "material.hpp"
#include "mesh.hpp"

class Scene
{
public:
    int maxDepth;
    Vec3 backgroundColor;
    Camera camera;

    AmbientLight ambient;
    std::vector<PointLight> pointLights;
    std::vector<TriangularLight> triangleLights;
    std::vector<Material> materials;
    std::vector<Mesh> meshes;

    void printInfo() const;
};

#endif
