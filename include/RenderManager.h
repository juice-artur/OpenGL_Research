#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <WindowManager.h>

class RenderManager
{
public:
    const static int SCREEN_WIDTH = 1920;
    const static int SCREEN_HEIGHT = 1080;

    bool StartUp(WindowManager &window);
    void ShutDown(); // TODO:: Cleanup
    void Render();

private:
    WindowManager* Window;
};
#endif