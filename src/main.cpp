#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <iostream>

void RenderFPSGraph()
{
    ImGui::Begin("FPS Graph");

    static float values[120] = {};
    static int valuesOffset = 0;
    values[valuesOffset] = ImGui::GetIO().Framerate;
    valuesOffset = (valuesOffset + 1) % 120;

    ImGui::PlotLines("FPS", values, 120, valuesOffset, nullptr, 0.0f, 100.0f, ImVec2(0, 80));
    float averageFPS = 0.0f;
    float maxFPS = FLT_MIN;
    float minFPS = FLT_MAX;
    for (int i = 0; i < 120; i++)
    {
        averageFPS += values[i];
        maxFPS = std::max(maxFPS, values[i]);
        minFPS = std::min(minFPS, values[i]);
    }
    averageFPS /= 120;

    ImGui::SameLine();

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.75f);
    ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() * 0.25f);
    ImGui::NextColumn();



    ImGui::Text("Max: %.1f", maxFPS);
    ImGui::TextColored(
        ImVec4(averageFPS <= 140 ? 1.0 : 0, averageFPS >= 140 ? 1.0 : 0, 0 , 1.0f), "Avg: %.1f", averageFPS);
    ImGui::Text("Min: %.1f", minFPS);


    ImGui::End();
}

void RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderFPSGraph();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()) return -1;

    window = glfwCreateWindow(1240, 980, "Hello Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;

    glClearColor(0.5, 1, 0.3, 1);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        RenderImGui();
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}