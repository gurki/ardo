#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "board.h"
#include "sound.h"


class Renderer
{
    public:
        
        Renderer() {};
    
        void setBoardFPV(const Board& board) const;
        void drawBoard(const Board& board) const;
        void drawBalls(const unordered_set<vec2i>& balls) const;
        void drawPath(const vector<vec2i>& path) const;
};


#endif
