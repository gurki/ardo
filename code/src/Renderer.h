#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "board.h"
#include "sound.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <array>


class Renderer
{
    public:
    
        typedef array<vec3f, 3> State;
    
        Renderer();
    
        void initOpenGL() const;
    
        void drawBoard(const Board& board) const;
        void drawPlayer(const vec2i& position) const;
        void drawBalls(const unordered_set<vec2i>& balls) const;
        void drawGuesses(const unordered_set<vec2i>& guesses) const;
        void drawMarker(const vec2i& marker) const;
        void drawPath(const vector<vec2i>& path) const;
    
        void setViewType(const bool firstPerson) { fpv_ = firstPerson; };
        void setViewActive(const Board& board) const;
    
        const bool isFirstPerson() const { return fpv_; };
        const State getPlayerState(const Board& board) const;
    
    
    private:
    
        bool fpv_;  //  first / third person view (fpv / tpv)
};


#endif
