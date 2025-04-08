#include "render.hpp"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

#include "camera.hpp"
#include "ray.hpp"
#include "vec3.hpp"

// === YANSIMA FONKSİYONU ===
Vec3 reflect(const Vec3 &I, const Vec3 &N)
{
    return I - N * 2.0f * I.dot(N);
}

// === IŞIN RENGİ HESAPLAMA ===
Vec3 computeColor(const Scene &scene, const Ray &ray, int depth)
{
    if (depth <= 0)
        return scene.backgroundColor;

    Intersection closestHit = {false, 1e9f};
    for (const Mesh &mesh : scene.meshes)
    {
        Intersection hit = mesh.intersect(ray, 0.001f, closestHit.t);
        if (hit.hit && hit.t < closestHit.t)
        {
            closestHit = hit;
        }
    }

    if (!closestHit.hit)
    {
        return scene.backgroundColor;
    }

    const Material &mat = scene.materials[closestHit.materialIndex];
    Vec3 finalColor = mat.ambient * scene.ambient.intensity;

    for (const PointLight &light : scene.pointLights)
    {
        Vec3 lightDir = (light.position - closestHit.point).normalize();
        float lightDist = (light.position - closestHit.point).length();

        Ray shadowRay(closestHit.point + lightDir * 0.001f, lightDir);
        bool inShadow = false;
        for (const Mesh &mesh : scene.meshes)
        {
            Intersection shadowHit = mesh.intersect(shadowRay, 0.001f, lightDist);
            if (shadowHit.hit)
            {
                inShadow = true;
                break;
            }
        }

        if (!inShadow)
        {
            float diff = std::max(0.0f, closestHit.normal.dot(lightDir));
            Vec3 diffuse = mat.diffuse * light.intensity * diff;

            Vec3 viewDir = -ray.direction.normalize();
            Vec3 reflectDir = reflect(-lightDir, closestHit.normal).normalize();
            float spec = std::pow(std::max(0.0f, viewDir.dot(reflectDir)), mat.phongExponent);
            Vec3 specular = mat.specular * light.intensity * spec;

            finalColor += diffuse + specular;
        }
    }

    for (const TriangularLight &triLight : scene.triangleLights)
    {
        Vec3 center = (triLight.v0 + triLight.v1 + triLight.v2) / 3.0f;
        Vec3 lightDir = (center - closestHit.point).normalize();
        float lightDist = (center - closestHit.point).length();

        Ray shadowRay(closestHit.point + lightDir * 0.001f, lightDir);
        bool inShadow = false;
        for (const Mesh &mesh : scene.meshes)
        {
            Intersection shadowHit = mesh.intersect(shadowRay, 0.001f, lightDist);
            if (shadowHit.hit)
            {
                inShadow = true;
                break;
            }
        }

        if (!inShadow)
        {
            float diff = std::max(0.0f, closestHit.normal.dot(lightDir));
            Vec3 diffuse = mat.diffuse * triLight.intensity * diff;

            Vec3 viewDir = -ray.direction.normalize();
            Vec3 reflectDir = reflect(-lightDir, closestHit.normal).normalize();
            float spec = std::pow(std::max(0.0f, viewDir.dot(reflectDir)), mat.phongExponent);
            Vec3 specular = mat.specular * triLight.intensity * spec;

            finalColor += diffuse + specular;
        }
    }

    if (mat.mirror.length() > 0.0f)
    {
        Vec3 reflectDir = reflect(ray.direction, closestHit.normal).normalize();
        Ray reflectRay(closestHit.point + reflectDir * 0.001f, reflectDir);
        Vec3 reflectedColor = computeColor(scene, reflectRay, depth - 1);
        finalColor += reflectedColor * mat.mirror;
    }

    finalColor.x = std::min(finalColor.x, 1.0f);
    finalColor.y = std::min(finalColor.y, 1.0f);
    finalColor.z = std::min(finalColor.z, 1.0f);

    return finalColor;
}

// === PPM RENDER FONKSİYONU ===
void render(const Scene &scene, const std::string &outputPath)
{
    int width = scene.camera.imageWidth;
    int height = scene.camera.imageHeight;
    std::vector<Vec3> framebuffer(width * height);

    int numThreads = std::thread::hardware_concurrency(); // ya da sabit bir sayı (ör: 4)
    if (numThreads == 0)
        numThreads = 4;
    // numThreads = 1;
    std::cout << "Number of threads: " << numThreads << std::endl;

    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; ++t)
    {
        threads.emplace_back([&, t]()
                             {
            for (int y = t; y < height; y += numThreads)
            {
                for (int x = 0; x < width; ++x)
                {
                    Ray ray = scene.camera.generateRay(x, y);
                    Vec3 color = computeColor(scene, ray, scene.maxDepth);
                    framebuffer[y * width + x] = color;
                }
            } });
    }

    for (auto &th : threads)
    {
        th.join();
    }

    std::ofstream ofs(outputPath);
    ofs << "P3\n"
        << width << " " << height << "\n255\n";

    for (const Vec3 &color : framebuffer)
    {
        int r = std::min(255, std::max(0, static_cast<int>(color.x * 255)));
        int g = std::min(255, std::max(0, static_cast<int>(color.y * 255)));
        int b = std::min(255, std::max(0, static_cast<int>(color.z * 255)));
        ofs << r << " " << g << " " << b << "\n";
    }

    ofs.close();
}
