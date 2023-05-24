#include "RenderManager.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>


bool RenderManager::StartUp(WindowManager& WindowManager)
{
    this->Window = &WindowManager;

    return true;
}

void RenderManager::Render()
{
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "DrawUI");
    ImGui::Begin("FPS Graph", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowSize(ImVec2(500, 150));
    ImGui::SetNextWindowPos(ImVec2(1240 - 500, 0), ImGuiCond_Always);
    ImGui::Text("Max: %.1f", 100);
    ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::NewFrame();
    glPopDebugGroup();

    Window->SwapWindow();
}
