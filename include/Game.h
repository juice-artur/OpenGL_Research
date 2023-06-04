#ifndef GAME_H
#define GAME_H
#include "WindowManager.h"
#include <RenderManager.h>
#include "Scene.h"

class Game
{
public:
    void Run();

    bool StartUp();

    Game(){};

   

private:
    WindowManager WindowManager;
    RenderManager RenderManager;
    static Scene CurentScene;
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    void ProcessInput(GLFWwindow* window, double DeltaTime);

    static float LastX;
    static float LastY;

    static bool FirstMouse;
};

#endif