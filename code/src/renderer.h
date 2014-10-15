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
        
        void drawBoard(const Board& board) const;
        void drawSound(const Sound& sound) const {};
};


#endif
