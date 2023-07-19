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
    ShadowPassShader = Shader("../resources/Shader/ShadowPass.vert", "../resources/Shader/ShadowPass.frag",
        "../resources/Shader/ShadowPass.geom");
    LightPassShader.Use();
    LightPassShader.SetInt("gPosition", 0);
    LightPassShader.SetInt("gNormal", 1);
    LightPassShader.SetInt("gColor", 2);
    LightPassShader.SetInt("shadowCubemap", 3);

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

    textureName = "gNormal";
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


    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    glGenFramebuffers(1, &shadowMapFBO);

    glGenTextures(1, &shadowCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_CULL_FACE);
    return true;
}

void RenderManager::Render(
    const std::vector<Mesh>& Meshes, const std::vector<Light>& Lights, glm::mat4 ViewMatrix, glm::vec3 CameraPosition, double DeltaTime)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);


            glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), (float)GLOBAL_CONSTANTS::SCREEN_WIDTH/ (float)GLOBAL_CONSTANTS::SCREEN_HEIGHT, 0.1f, 100.0f);

    GeometryPassShader.Use();
    GeometryPassShader.SetMat4("ViewMatrix", ViewMatrix);
    GeometryPassShader.SetMat4("ProjectionMatrix", projection);

    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "GeometryPass");
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto currentMesh : Meshes)
    {
        Material currentMaterial;
        if (!currentMesh.materials.empty())
        {
            currentMaterial = currentMesh.materials[0];
        }
        else
        {
            currentMaterial = Material{};
        }

        GeometryPassShader.SetMat4("Model", currentMesh.GetModelMatrix());
        GeometryPassShader.SetVec4("Color", currentMesh.Color);
        DrawMesh(currentMesh, currentMaterial);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPopDebugGroup();

    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "ShadowPass");
    float near_plane = 1.0f;
    float far_plane = 25.0f;
    glm::mat4 shadowProj = glm::perspective(
        glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[0].GetPosition(), Lights[0].GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f),
                                                glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[0].GetPosition(), Lights[0].GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f),
                                                glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[0].GetPosition(), Lights[0].GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f),
                                                glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[0].GetPosition(), Lights[0].GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f),
                                                glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[0].GetPosition(), Lights[0].GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f),
                                                glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[0].GetPosition(), Lights[0].GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f),
                                                glm::vec3(0.0f, -1.0f, 0.0f)));

    glViewport(0, 0, 1024, 1024);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    ShadowPassShader.Use();
    for (unsigned int i = 0; i < 6; ++i)
    {
        ShadowPassShader.SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    ShadowPassShader.SetFloat("far_plane", far_plane);
    ShadowPassShader.SetVec3("lightPos", Lights[0].GetPosition());

    for (auto curentMesh : Meshes)
    {
        ShadowPassShader.SetMat4("Model", curentMesh.GetModelMatrix());

        glBindVertexArray(curentMesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)curentMesh.vertices.size());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
    glPopDebugGroup();

    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "LightPass");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    LightPassShader.Use();
    glViewport(0, 0, GLOBAL_CONSTANTS::SCREEN_WIDTH, GLOBAL_CONSTANTS::SCREEN_HEIGHT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColor);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemap);


    LightPassShader.SetVec3("lightPos", Lights[0].GetPosition());
    LightPassShader.SetVec3("viewPos", CameraPosition);
    LightPassShader.SetFloat("far_plane", far_plane);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, GLOBAL_CONSTANTS::SCREEN_WIDTH, GLOBAL_CONSTANTS::SCREEN_HEIGHT, 0, 0, GLOBAL_CONSTANTS::SCREEN_WIDTH,
        GLOBAL_CONSTANTS::SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPopDebugGroup();

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

void RenderManager::DrawMesh(Mesh mesh, Material curentMaterial)
{
    GeometryPassShader.Use();
    GeometryPassShader.SetInt("DiffuseTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curentMaterial.diffuseTextureID);

    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.vertices.size());
}
