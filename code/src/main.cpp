#include "common.h"
#include "Game.h"


int main(int argc, char* argv[])
{
    //  create window
    sf::VideoMode mode = sf::VideoMode(1440, 900);
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
    
    //  create and start game
    Game game(&window, mode, title, settings);
    
    while (game.isRunning())
    {
        game.handleEvents();
        game.update();
        game.render();
    }
    
    return 0;
}