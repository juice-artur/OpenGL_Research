#include <vector>
#include <Mesh.h>

#ifndef SCENE_H
#define SCENE_H
class Scene
{
public:
    bool StartUp();
    const std::vector<Mesh>& GetMeshes();

private:
    std::vector<Mesh> Meshes;
};
#endif