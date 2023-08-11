#include <vector>
#include <Mesh.h>
#include <Camera.h>
#include <Light.h>
#include <Model.h>
#ifndef SCENE_H
#define SCENE_H
class Scene
{
public:
    bool StartUp();
    const std::vector<Mesh>& GetMeshes();
    const std::vector<Light>& GetLights();
    Camera MainCamera;
    void Update(float DeltaTime);

private:
    std::vector<Mesh> Meshes;
    std::vector<Light> Lights;

};
#endif