#include "Game.h"
#include <iostream>
#include <imgui.h>
#include <Constants.h>

float Game::LastX = 0;
float Game::LastY = 0;
bool Game::FirstMouse = true;
Scene Game::CurentScene;

void Game::Run()
{
    double PreviousTime = glfwGetTime();
    double DeltaTime = 0;

    while (!glfwWindowShouldClose(WindowManager.GetWindow()))
    {
        double CurrentTime = glfwGetTime();
        ProcessInput(WindowManager.GetWindow(), DeltaTime);

        RenderManager.Render(CurentScene.GetMeshes(), CurentScene.GetLights(), CurentScene.MainCamera.GetViewMatrix(), DeltaTime);

        DeltaTime = (CurrentTime - PreviousTime) * 1000.0;

        PreviousTime = CurrentTime;
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

    glfwSetCursorPosCallback(WindowManager.GetWindow(), Game::MouseCallback);
    return true;
}

void Game::ProcessInput(GLFWwindow* window, double DeltaTime)
{
    ImGuiIO& io = ImGui::GetIO();
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    const float cameraSpeed = 2.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        CurentScene.MainCamera.ProcessKeyboard(FORWARD, SPEED * DeltaTime / 1000);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        CurentScene.MainCamera.ProcessKeyboard(BACKWARD, SPEED * DeltaTime / 1000);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        CurentScene.MainCamera.ProcessKeyboard(LEFT, SPEED * DeltaTime / 1000);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        CurentScene.MainCamera.ProcessKeyboard(RIGHT, SPEED * DeltaTime / 1000);
    }
}

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (FirstMouse)
    {
        LastX = xpos;
        LastY = ypos;
        FirstMouse = false;
    }

    float xoffset = xpos - LastX;
    float yoffset = LastY - ypos;

    LastX = xpos;
    LastY = ypos;

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    if (cursorX < 0.0 || cursorX >= GLOBAL_CONSTANTS::SCREEN_WIDTH || cursorY < 0.0 || cursorY >= GLOBAL_CONSTANTS::SCREEN_HEIGHT)
    {
        double centerX = GLOBAL_CONSTANTS::SCREEN_WIDTH / 2.0;
        double centerY = GLOBAL_CONSTANTS::SCREEN_HEIGHT / 2.0;
        glfwSetCursorPos(window, centerX, centerY);

        LastX = centerX;
        LastY = centerY;
    }

    CurentScene.MainCamera.ProcessMouseMovement(xoffset, yoffset);
}
