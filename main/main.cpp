#include "userio.h"
#include "textures.h"
#include "gameState.h"

#include <iostream> // Printing information about texture loading fail

int main()
{
    // Loading textures
    if (!Textures::init())
    {
        std::cerr << "Loading textures failed\n";
        return -1;
    }
    Image::init();

    // User input via mouse and output via screen
    UserIO userIO("Backrooms");

    GameState game;

    // Main loop
    while (userIO.isOpen())
    {
        // Handling events
        userIO.handleEvents();

        game.step(userIO);
    }

    return 0;
}
