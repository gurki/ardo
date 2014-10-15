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
    
        Board(const int width = 8,
              const int height = 8,
              const int nballs = 5);
    
        void init(const int width = 8,
                  const int height = 8);
        void initBallsRandom(const int nballs = 5);
    
        void movePlayerRight();
        void movePlayerLeft();
    
        void shoot();
    
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
