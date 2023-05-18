#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    const static int SCREEN_WIDTH = 1920;
    const static int SCREEN_HEIGHT = 1080;


    bool StartUp();
    void ShutDown();

private:
    GLFWwindow* window;

    bool InitializeOpenGL();
    bool CreateWindow();
    bool CreateGLContext();
    bool CreateImGuiContext();
};

#endif