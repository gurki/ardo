#include "board.h"

#include <random>


////////////////////////////////////////////////////////////////////////////////
Board::Board(const int width,
             const int height,
             const int nballs)
{
    init(width, height);
    initBallsRandom(nballs);
    
    _playerId = 0;
}


////////////////////////////////////////////////////////////////////////////////
void Board::init(const int width,
                 const int height)
{
    _balls.clear();
    _width = width;
    _height = height;
}


////////////////////////////////////////////////////////////////////////////////
void Board::initBallsRandom(const int nballs)
{
    _balls.clear();
    
    //  keep spwaning random balls until we have enough
    while (_balls.size() <= nballs)
    {
        vec2i pos;
        pos.x = rand() % _width;
        pos.y = rand() % _height;
        
        //  only add ball if position is free
        if (_balls.find(pos) != _balls.end()) {
            _balls.insert(pos);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void Board::movePlayerRight() { _playerId++; }


////////////////////////////////////////////////////////////////////////////////
void Board::movePlayerLeft() { _playerId--; }


////////////////////////////////////////////////////////////////////////////////
void Board::shoot() {}


////////////////////////////////////////////////////////////////////////////////
Board::Side Board::getPlayerSide() // takes the position and returns the side
{
    int r;
    r=_playerId % (2*(_width+_height)); //in case we did the full circle around the board
    Side s;
    if (r<_width) {
        s=South;
    }
    else if (r<_width+_height){
        s=West;
    }
    else if (r<2*_width+_height){
        s=North;
    }
    else if (r<2*(_width+_height)){
        s=East;
    }
    return s;
}


////////////////////////////////////////////////////////////////////////////////
int Board::getPlayerPosition()
{
    int r;
    Side s;
    r=_playerId % (2*(_width+_height));
    s=getPlayerSide();
    if(s==West){
        r=r-_width;
    }
    else if (s==North){
        r=r-_width-_height;
    }
    else if (s==East){
        r=r-2*_width-_height;
    }
    return r;
}

////////////////////////////////////////////////////////////////////////////////
vector<vec2i> Board::getPath()
{
    return vector<vec2i>();
}