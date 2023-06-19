#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <WindowManager.h>
#include "Mesh.h"
#include <Shader.h>
#include <Light.h>

class RenderManager
{
public:
    RenderManager(){};

    bool StartUp(WindowManager& window);
    void ShutDown();  // TODO:: Cleanup
    void Render(const std::vector<Mesh>& Meshes, const std::vector<Light>& Lights, glm::mat4 ViewMatrix, glm::vec3 CameraPosition,
        double DeltaTime);

private:
    unsigned int gBuffer;
    unsigned int gPosition, gNormal, gColor;
    WindowManager* Window;
    Shader GeometryPassShader;
    Shader LightPassShader;
    void DrawMesh(Mesh mesh);
};
#endif