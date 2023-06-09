#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
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