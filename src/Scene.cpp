#include "Scene.h"

bool Scene::StartUp()
{
    Mesh Bunny("../resources/bunny/bunny.obj");
    Bunny.SetPosition({5, 0, 5});
    Meshes.push_back(Bunny);
    Mesh Cube("../resources/Cube/cube.obj");
    Cube.ScaleMesh({10, 0.2, 10});
    Cube.SetPosition({5, -1, 5});
    Cube.SetColor({0.0f, 0.3f, 0.8f, 1.0f});
    Meshes.push_back(Cube);

    Light PointLight(Mesh("../resources/Sphere/Sphere.obj"), {40.0f, 40.0f, 40.0f}, {1.0f, 1.0f, 1.0, 1.0f});
    Lights.push_back(PointLight);
    MainCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    return true;
}

const std::vector<Mesh>& Scene::GetMeshes()
{
    return Meshes;
}

const std::vector<Light>& Scene::GetLights()
{
    return Lights;
}
