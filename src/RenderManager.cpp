#include "RenderManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <Constants.h>

bool RenderManager::StartUp(WindowManager& WindowManager)
{
    this->Window = &WindowManager;
    GeometryPassShader = Shader("../resources/Shader/GeometryPass.vert", "../resources/Shader/GeometryPass.frag");
    LightPassShader = Shader("../resources/Shader/LightPass.vert", "../resources/Shader/LightPass.frag");
    LightPassShader.Use();
    LightPassShader.SetInt("gPosition", 0);
    LightPassShader.SetInt("gNormal", 1);
    LightPassShader.SetInt("gColor", 2);

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    const char* bufferName = "MyBuffer";
    glObjectLabel(GL_BUFFER, gBuffer, -1, bufferName);

    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GLOBAL_CONSTANTS::SCREEN_WIDTH, GLOBAL_CONSTANTS::SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    const char* textureName = "gPosition";
    glObjectLabel(GL_TEXTURE, gPosition, -1, textureName);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GLOBAL_CONSTANTS::SCREEN_WIDTH, GLOBAL_CONSTANTS::SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    glGenTextures(1, &gColor);
    glBindTexture(GL_TEXTURE_2D, gColor);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, GLOBAL_CONSTANTS::SCREEN_WIDTH, GLOBAL_CONSTANTS::SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColor, 0);
    textureName = "gColor";
    glObjectLabel(GL_TEXTURE, gColor, -1, textureName);

    textureName = "MyTexture";
    glObjectLabel(GL_TEXTURE, gNormal, -1, textureName);

    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GLOBAL_CONSTANTS::SCREEN_WIDTH, GLOBAL_CONSTANTS::SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void RenderManager::Render(
    const std::vector<Mesh>& Meshes, const std::vector<Light>& Lights, glm::mat4 ViewMatrix, glm::vec3 CameraPosition, double DeltaTime)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjectionMatrix =
        glm::perspective(glm::radians(45.0f), (float)GLOBAL_CONSTANTS::SCREEN_WIDTH / (float)GLOBAL_CONSTANTS::SCREEN_HEIGHT, 0.1f, 100.0f);


    GeometryPassShader.Use();
    GeometryPassShader.SetMat4("ViewMatrix", ViewMatrix);
    GeometryPassShader.SetMat4("ProjectionMatrix", ProjectionMatrix);



    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "GeometryPass");
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto curentMesh : Meshes)
    {
        GeometryPassShader.SetMat4("Model", curentMesh.GetModelMatrix());
        GeometryPassShader.SetVec4("Color", curentMesh.Color);
        DrawMesh(curentMesh);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPopDebugGroup();

    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "LightPass");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    LightPassShader.Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColor);

    for (unsigned int i = 0; i < Lights.size(); i++)
    {
        LightPassShader.SetVec3("lights[" + std::to_string(i) + "].Position", Lights[i].GetPosition());
        LightPassShader.SetVec4("lights[" + std::to_string(i) + "].Color", Lights[i].GetColor());

        LightPassShader.SetFloat("lights[" + std::to_string(i) + "].Linear", Lights[i].GetLinear());
        LightPassShader.SetFloat("lights[" + std::to_string(i) + "].Quadratic", Lights[i].GetQuadratic());

        const float maxBrightness = std::fmaxf(std::fmaxf(Lights[i].GetColor().r, Lights[i].GetColor().g), Lights[i].GetColor().b);
        float radius = Lights[i].GetRadius();
        LightPassShader.SetFloat("lights[" + std::to_string(i) + "].Radius", radius);
    }
    LightPassShader.SetVec3("viewPos", CameraPosition);

    glPopDebugGroup();


    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, GLOBAL_CONSTANTS::SCREEN_WIDTH, GLOBAL_CONSTANTS::SCREEN_HEIGHT, 0, 0, GLOBAL_CONSTANTS::SCREEN_WIDTH,
        GLOBAL_CONSTANTS::SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   //
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,    //
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   //
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);


    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "DrawUI");
    ImGui::Begin("FPS Graph", nullptr);
    ImGui::SetWindowSize(ImVec2(500, 150));
    ImGui::SetNextWindowPos(ImVec2(GLOBAL_CONSTANTS::SCREEN_WIDTH - 500, 0), ImGuiCond_Always);
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
    GeometryPassShader.Use();
    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.vertices.size());
}
