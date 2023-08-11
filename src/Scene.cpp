#include "Scene.h"
#include <GLFW/glfw3.h>


bool Scene::StartUp()
{

    Model Bunny("../resources/bunny/bunny.obj");
    Bunny.SetPosition({2, 0.5, 1});
    Bunny.SetColor({0, 255, 0, 1.0f});
    std::copy(Bunny.meshes.begin(), Bunny.meshes.end(), std::back_inserter(Meshes));
    Model Cube("../resources/Cube/cube.obj");
    Cube.ScaleModel({100, 0.2, 100});
    Cube.SetPosition({50.0f, 0.0f, 50.0});
    Cube.SetColor({0, 0, 255, 1.0f});
    std::copy(Cube.meshes.begin(), Cube.meshes.end(), std::back_inserter(Meshes));
    Model Air_Gun("../resources/AirgunHaenel/Air_Gun-Wavefront OBJ.obj");
    Air_Gun.SetPosition({3.0f, 3.0f, 2.0});
    Air_Gun.RotateModel({0, 90, -90});
    std::copy(Air_Gun.meshes.begin(), Air_Gun.meshes.end(), std::back_inserter(Meshes));

    Model Air_GunV2("../resources/AirgunHaenel/Air_Gun-Wavefront OBJ.obj");
    Air_GunV2.SetPosition({3.0f, 1.0f, -2.0});
    Air_GunV2.ScaleModel({3, 3, 3});
    std::copy(Air_GunV2.meshes.begin(), Air_GunV2.meshes.end(), std::back_inserter(Meshes));

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

void Scene::Update(float DeltaTime)
{
   Meshes[2].RotateMesh({0, sin(glfwGetTime()) * 1000, -90});
}
