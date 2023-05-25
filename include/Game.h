#ifndef GAME_H
#define GAME_H
#include "WindowManager.h"
#include <RenderManager.h>

class Game
{
public:
    void Run();

    bool StartUp();


private:
    WindowManager WindowManager;
    RenderManager RenderManager;

    void ProcessInput(GLFWwindow* window);
};


#endif