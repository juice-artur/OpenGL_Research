#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <WindowManager.h>
#include "Mesh.h"
#include <Shader.h>

class RenderManager
{
public:
    RenderManager(){};

    bool StartUp(WindowManager &window);
    void ShutDown(); // TODO:: Cleanup
    void Render(const std::vector<Mesh>& Meshes, glm::mat4 ViewMatrix, double DeltaTime);

private:
    WindowManager* Window;
    Shader MeshShader;
    void DrawMesh(Mesh mesh);
};
#endif