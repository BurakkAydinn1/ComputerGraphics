#include "scene.hpp"
#include <iostream>

void Scene::printInfo() const {
    std::cout << "Max Ray Depth: " << maxDepth << "\n";
    std::cout << "Background: "; backgroundColor.print();
    std::cout << "Camera position: "; camera.position.print();
    std::cout << "Ambient Light: "; ambient.intensity.print();
    std::cout << "Materials count: " << materials.size() << "\n";
    std::cout << "Point Lights: " << pointLights.size() << "\n";
    std::cout << "Triangular Lights: " << triangleLights.size() << "\n";
}
