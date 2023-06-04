#include <vector>
#include <Mesh.h>
#include <Camera.h>

#ifndef SCENE_H
#define SCENE_H
class Scene
{
public:
    bool StartUp();
    const std::vector<Mesh>& GetMeshes();
    Camera MainCamera;

private:
    std::vector<Mesh> Meshes;
};
#endif