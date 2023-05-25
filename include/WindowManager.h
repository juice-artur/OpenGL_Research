#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
    const static int SCREEN_WIDTH = 1920;
    const static int SCREEN_HEIGHT = 1080;

    bool StartUp();
    void ShutDown();
    void SwapWindow();

    GLFWwindow* GetWindow() const;

private:
    GLFWwindow* window;

    bool InitializeOpenGL();
    bool CreateWindow();
    bool CreateGLContext();
    bool CreateImGuiContext();
};

#endif