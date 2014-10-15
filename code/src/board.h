#ifndef BOARD_H
#define BOARD_H

#include "vector.h"

#include <set>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
class Board
{
    public:
    
        enum Side {
            North,
            East,
            South,
            West
        };
    
        enum Direction {
            Up,
            Right,
            Left,
            Down
        };
    
        Board(const int width, const int height, const int nballs = 0);
    
        void init(const int width, const int height);
        void initBallsRandom(const int nballs);
    
        void addBall(const vec2i& pos);
        void removeBall(const vec2i& pos);
    
        void movePlayerRight();
        void movePlayerLeft();
        void setPlayerPosition(const int pos) { playerId_ = pos; };
        void shoot();
    
        const int getWidth() const { return width_; };
        const int getHeight() const { return height_; };
    
        const Side getPlayerSide() const;
        const int getPlayerPosition() const;
        const vec2i getPlayerCoordinates() const;
        const Direction getPlayerDirection() const;

        const bool isBall(const vec2i& pos) const;
        const bool isBorder(const vec2i& pos) const;
    
        vector<vec2i> getPath();
    
    
    private:
    
        set<vec2i> balls_;
    
        int playerId_;
        int width_;
        int height_;
};


#endif
