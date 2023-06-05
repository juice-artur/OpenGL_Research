#include "RenderManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

bool RenderManager::StartUp(WindowManager& WindowManager)
{
    this->Window = &WindowManager;
    MeshShader = Shader("../resources/Shader/BaseShader.vert", "../resources/Shader/BaseShader.frag");
    return true;
}

void RenderManager::Render(const std::vector<Mesh>& Meshes, glm::mat4 ViewMatrix, double DeltaTime)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    MeshShader.SetMat4("ViewMatrix", ViewMatrix);
    MeshShader.SetMat4("ProjectionMatrix", ProjectionMatrix);

    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Draw Mesh");
    for (auto curentMesh : Meshes)
    {
        MeshShader.SetMat4("Model", glm::mat4(1));
        DrawMesh(curentMesh);
    }
    glPopDebugGroup();


    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "DrawUI");
    ImGui::Begin("FPS Graph", nullptr);
    ImGui::SetWindowSize(ImVec2(500, 150));
    ImGui::SetNextWindowPos(ImVec2(1240 - 500, 0), ImGuiCond_Always);
    ImGui::Text("Max: %.1f", DeltaTime);
    ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::NewFrame();
    glPopDebugGroup();

    Window->SwapWindow();
}

void RenderManager::DrawMesh(Mesh mesh)
{
    MeshShader.Use();
    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.vertices.size());
}
