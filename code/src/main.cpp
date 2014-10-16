#include "common.h"
#include "Game.h"

#include <limits>
#include <deque>


int main(int argc, char* argv[])
{
    //  create window
    //      this must be done first, so that the opengl context can initialise
    //      before the Game() constructor initialises any rendering states
    sf::VideoMode mode = sf::VideoMode(1920, 1080);
    string title = "ORDO";
    sf::ContextSettings settings = sf::ContextSettings(32);
    
    sf::Window window(
        mode,
        title,
        sf::Style::Default,
        settings
    );

    window.setVerticalSyncEnabled(true);
    window.setActive();
    
    //  create and run game
    Game game(&window, mode, title, settings);
    
    while (game.isRunning())
    {
        game.handleEvents();
        game.update();
        game.render();
    }
    
    return 0;
}








