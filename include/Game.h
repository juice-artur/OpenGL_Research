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
    Scene CurentScene;

    void ProcessInput(GLFWwindow* window);
};


#endif