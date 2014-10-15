#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "vector.h"

#include <unordered_set>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
class Board
{
    public:
    
        enum Side {         //  order is important!
            North,
            East,
            South,
            West
        };
    
        enum Direction {    //  order is important!
            Down,
            Left,
            Up,
            Right
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
        const vec2i getPlayerCenter() const;
        const vec2i getPlayerEyes() const;
        const Direction getPlayerDirection() const;

        const bool isBall(const vec2i& pos) const;
        const bool isBorder(const vec2i& pos) const;
    
        const vector<vec2i> getPath() const;
        const unordered_set<vec2i>& getBalls() const { return balls_; };
    
    private:
    
        unordered_set<vec2i> balls_;
    
        int playerId_;
        int width_;
        int height_;
};


ostream& operator << (ostream& out, const Board::Side& side);
ostream& operator << (ostream& out, const Board::Direction& dir);
ostream& operator << (ostream& out, const vector<vec2i>& v);


#endif
