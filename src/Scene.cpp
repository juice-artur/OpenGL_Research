#include "Scene.h"

bool Scene::StartUp()
{
    Meshes.push_back("../resources/Crate/Crate1.obj");
    MainCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    return true;
}

const std::vector<Mesh>& Scene::GetMeshes()
{
    return Meshes;
}
