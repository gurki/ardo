#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "board.h"
#include "sound.h"


class Renderer
{
    public:
        
        Renderer() {};
        
        void drawBoard(const sf::Window& window, const Board& board) {};
        void drawSound(const sf::Window& window, const Sound& sound) {};
};


#endif
