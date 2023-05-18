#include "Game.h"
#include <iostream>

void Game::Run() 
{

}

bool Game::StartUp()
{
    if (!window.StartUp())
    {
        std::cout << "Failed to initialize window display manager." << std::endl;
        return false;
    }
}
