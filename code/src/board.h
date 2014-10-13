#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <unordered_set>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
template <class T>
class vec2
{
    public:
    
        vec2(const T x_ = 0, const T y_ = 0) : x(x_), y(y_) {};
    
        T x;
        T y;
};


typedef vec2<int> vec2i;


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
    
        Board(const int width = 8,
              const int height = 8,
              const int nballs = 5);
    
        void init(const int width = 8,
                  const int height = 8);
        void initBallsRandom(const int nballs = 5);
    
        void movePlayerRight();
        void movePlayerLeft();
    
        void shoot();
    
        Side getPlayerSide();
        int getPlayerPosition();
    
        vector<vec2i> getPath();
    
    
    private:
    
        unordered_set<vec2i> _balls;
    
        int _playerId;
        int _width;
        int _height;
};


#endif
