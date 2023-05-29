#include "Game.h"
#include <iostream>
#include <imgui.h>

void Game::Run()
{
    while (!glfwWindowShouldClose(WindowManager.GetWindow()))
    {
        ProcessInput(WindowManager.GetWindow());

        RenderManager.Render(CurentScene.GetMeshes());
    }
}

bool Game::StartUp()
{
    if (!WindowManager.StartUp())
    {
        std::cout << "Failed to initialize window." << std::endl;
        return false;
    }

    if (!RenderManager.StartUp(WindowManager))
    {
        printf("Failed to initialize Render manager.\n");
        return false;
    }

    if (!CurentScene.StartUp())
    {
        std::cout << "Failed to initialize Scene." << std::endl;
        return false;
    }
}


void Game::ProcessInput(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
