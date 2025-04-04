
#ifndef RENDER_HPP
#define RENDER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // min/max
#include <thread>
#include "camera.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "xmlparser.hpp"
#include "scene.hpp"

Vec3 reflect(const Vec3 &I, const Vec3 &N);
void render(const Scene &scene, const std::string &outputPath);
Vec3 computeColor(const Scene &scene, const Ray &ray, int depth);

#endif
