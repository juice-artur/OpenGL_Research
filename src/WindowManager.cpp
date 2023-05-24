#include "WindowManager.h"
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

bool WindowManager::CreateWindow()
{
    if (!glfwInit())
    {
        return false;
    }

    window = glfwCreateWindow(1240, 980, "Hello Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    return true;
}

bool WindowManager::CreateGLContext()
{
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    return true;
}

bool WindowManager::CreateImGuiContext()
{
    ImGuiContext* mGuiContext = ImGui::CreateContext();
    if (mGuiContext == nullptr)
    {
        std::cout << "Could not load IMGUI context!" << std::endl;
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return true;
}

bool WindowManager::StartUp()
{

    if (!CreateWindow())
    {
        return false;
    }

    if (!CreateGLContext())
    {
        return false;
    }

    if (!CreateImGuiContext())
    {
        return false;
    }

    return true;
}

void WindowManager::ShutDown()
{
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void WindowManager::SwapWindow()
{

    glfwSwapBuffers(window);
}

bool WindowManager::InitializeOpenGL()
{
    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
        return false;
    }
    return true;
}
