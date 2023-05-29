#include "Scene.h"

bool Scene::StartUp()
{
    Meshes.push_back("../resources/Crate/Crate1.obj");
    return true;
}

const std::vector<Mesh>& Scene::GetMeshes()
{
    return Meshes;
}
