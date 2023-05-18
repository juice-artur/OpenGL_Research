#ifndef GAME_H
#define GAME_H
#include "Window.h"

class Game
{
public:
    void Run();

    bool StartUp();

private:
    Window window;
};


#endif