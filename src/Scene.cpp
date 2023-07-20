#include "Scene.h"

bool Scene::StartUp()
{
    Mesh Bunny("../resources/bunny/bunny.obj");
    Bunny.SetPosition({2, 0.5, 1});
    Bunny.SetColor({0, 255, 0, 1.0f});
    Meshes.push_back(Bunny);
    Mesh Cube("../resources/Cube/cube.obj");
    Cube.ScaleMesh({100, 0.2, 100});
    Cube.SetPosition({-1.0f, 0.0f, 2.0});
    Cube.SetColor({0, 0, 255, 1.0f});
    Meshes.push_back(Cube);
    Mesh Air_Gun("../resources/AirgunHaenel/Air_Gun-Wavefront OBJ.obj");
    Air_Gun.SetPosition({3.0f, 3.0f, 2.0});
    Meshes.push_back(Air_Gun);

    Light PointLight(Mesh("../resources/Sphere/Sphere.obj"), {-5.0f, 5.0f, 8.0f}, {1.0f, 1.0f, 1.0, 1.0f});
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
