#include "Game.h"
#include <iostream>

void Game::Run()
{
    while (true)
    {
        RenderManager.Render();
    }
}

bool Game::StartUp()
{
    if (!WindowManager.StartUp())
    {
        std::cout << "Failed to initialize window." << std::endl;
        return false;
    }

    if (!RenderManager.StartUp(WindowManager))
    {
        printf("Failed to initialize Render manager.\n");
        return false;
    }
}
