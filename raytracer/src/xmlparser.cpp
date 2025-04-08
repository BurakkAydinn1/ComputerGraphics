// Null-safe parseFromFile implementation
#include "xmlparser.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "tinyxml2.h"

using namespace tinyxml2;

static Vec3 parseVec3(const char *text)
{
    float x = 0, y = 0, z = 0;
    if (text)
        sscanf(text, "%f %f %f", &x, &y, &z);
    return Vec3(x, y, z);
}

static std::string safeText(XMLElement *e)
{
    return e && e->GetText() ? std::string(e->GetText()) : "";
}

Scene XMLParser::parseFromFile(const std::string &filepath)
{
    Scene scene;
    XMLDocument doc;
    if (doc.LoadFile(filepath.c_str()) != XML_SUCCESS)
    {
        std::cerr << "Error: cannot load scene file: " << filepath << "\n";
        return scene;
    }

    XMLElement *root = doc.FirstChildElement("scene");
    if (!root)
        return scene;

    // Max depth & background
    scene.maxDepth = std::stoi(safeText(root->FirstChildElement("maxraytracedepth")));
    scene.backgroundColor = parseVec3(safeText(root->FirstChildElement("background")).c_str());

    // Camera
    XMLElement *cam = root->FirstChildElement("camera");
    if (cam)
    {
        scene.camera.position = parseVec3(safeText(cam->FirstChildElement("position")).c_str());
        scene.camera.gaze = parseVec3(safeText(cam->FirstChildElement("gaze")).c_str());
        scene.camera.up = parseVec3(safeText(cam->FirstChildElement("up")).c_str());

        float l = -1, r = 1, b = -1, t = 1;
        sscanf(safeText(cam->FirstChildElement("nearplane")).c_str(), "%f %f %f %f", &l, &r, &b, &t);
        scene.camera.left = l;
        scene.camera.right = r;
        scene.camera.bottom = b;
        scene.camera.top = t;

        scene.camera.nearDistance = std::stof(safeText(cam->FirstChildElement("neardistance")));

        int w = 512, h = 512;
        sscanf(safeText(cam->FirstChildElement("imageresolution")).c_str(), "%d %d", &w, &h);
        scene.camera.imageWidth = w;
        scene.camera.imageHeight = h;
        scene.camera.initialize();
    }

    // Lights
    XMLElement *lights = root->FirstChildElement("lights");
    if (lights)
    {
        scene.ambient.intensity = parseVec3(safeText(lights->FirstChildElement("ambientlight")).c_str());

        for (XMLElement *pl = lights->FirstChildElement("pointlight"); pl; pl = pl->NextSiblingElement("pointlight"))
        {
            PointLight light;
            light.position = parseVec3(safeText(pl->FirstChildElement("position")).c_str());
            light.intensity = parseVec3(safeText(pl->FirstChildElement("intesity")).c_str());
            scene.pointLights.push_back(light);
        }

        for (XMLElement *tl = lights->FirstChildElement("triangularlight"); tl; tl = tl->NextSiblingElement("triangularlight"))
        {
            TriangularLight light;
            light.v0 = parseVec3(safeText(tl->FirstChildElement("vertex1")).c_str());
            light.v1 = parseVec3(safeText(tl->FirstChildElement("vertex2")).c_str());
            light.v2 = parseVec3(safeText(tl->FirstChildElement("vertex3")).c_str());
            light.intensity = parseVec3(safeText(tl->FirstChildElement("intensity")).c_str());
            scene.triangleLights.push_back(light);
        }
    }

    // Materials with id mapping
    std::unordered_map<std::string, int> matIdMap;
    XMLElement *materials = root->FirstChildElement("materials");
    if (materials)
    {
        int idx = 0;
        for (XMLElement *mat = materials->FirstChildElement("material"); mat; mat = mat->NextSiblingElement("material"))
        {
            std::string id = mat->Attribute("id") ? mat->Attribute("id") : std::to_string(idx);
            Material m;
            m.ambient = parseVec3(safeText(mat->FirstChildElement("ambient")).c_str());
            m.diffuse = parseVec3(safeText(mat->FirstChildElement("diffuse")).c_str());
            m.specular = parseVec3(safeText(mat->FirstChildElement("specular")).c_str());
            m.mirror = parseVec3(safeText(mat->FirstChildElement("mirrorreflactance")).c_str());
            m.phongExponent = std::stof(safeText(mat->FirstChildElement("phongexponent")));
            m.textureFactor = std::stof(safeText(mat->FirstChildElement("texturefactor")));
            scene.materials.push_back(m);
            matIdMap[id] = idx++;
        }
    }

    // Vertex data
    std::vector<Vec3> vertices;
    std::istringstream vstream(safeText(root->FirstChildElement("vertexdata")));
    while (!vstream.eof())
    {
        float x, y, z;
        vstream >> x >> y >> z;
        if (!vstream.fail())
            vertices.emplace_back(x, y, z);
    }

    // Mesh objects
    XMLElement *objects = root->FirstChildElement("objects");
    if (objects)
    {
        for (XMLElement *mesh = objects->FirstChildElement("mesh"); mesh; mesh = mesh->NextSiblingElement("mesh"))
        {
            std::string matId = safeText(mesh->FirstChildElement("materialid"));
            int materialIndex = matIdMap.count(matId) ? matIdMap[matId] : 0;

            Mesh m;
            std::istringstream fstream(safeText(mesh->FirstChildElement("faces")));
            std::string v0, v1, v2;

            while (fstream >> v0 >> v1 >> v2)
            {
                auto extract = [](const std::string &s)
                {
                    size_t pos = s.find('/');
                    return std::stoi(s.substr(0, pos)) - 1;
                };

                int i0 = extract(v0);
                int i1 = extract(v1);
                int i2 = extract(v2);
                if (i0 >= 0 && i1 >= 0 && i2 >= 0 && i0 < vertices.size() && i1 < vertices.size() && i2 < vertices.size())
                {
                    m.addTriangle(Triangle(vertices[i0], vertices[i1], vertices[i2], materialIndex));
                }
            }

            scene.meshes.push_back(m);
        }
    }

    return scene;
}
