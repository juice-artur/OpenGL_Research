#include <vector>
#include <Mesh.h>
#include <Camera.h>
#include <Light.h>

#ifndef SCENE_H
#define SCENE_H
class Scene
{
public:
    bool StartUp();
    const std::vector<Mesh>& GetMeshes();
    const std::vector<Light>& GetLights();
    Camera MainCamera;

private:
    std::vector<Mesh> Meshes;
    std::vector<Light> Lights;

};
#endif