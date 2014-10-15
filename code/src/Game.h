#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "Renderer.h"
#include "SoundRenderer.h"
#include "Board.h"

#include <unordered_map>

using namespace std;


class Game
{
    public:
    
        Game(
             sf::Window* window,
             const sf::VideoMode& mode,
             const string& title,
             const sf::ContextSettings& settings
        );
    
        void update();
        void render();
        void handleEvents();
    
        const bool isRunning() const { return isRunning_; };

    
    private:
    
        void handleKeyboardEvents(const sf::Event& event);
    
        sf::Window* window_;
        string windowTitle_;
        sf::VideoMode windowMode_;
        sf::ContextSettings windowSettings_;
    
        Board board_;
        Renderer renderer_;
        SoundRenderer soundRenderer_;
    
        sf::Clock clock_;
        bool isRunning_;
        unordered_map<string, bool> flags_;
};


#endif
