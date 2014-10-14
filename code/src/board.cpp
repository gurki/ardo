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
    vec2i current_point;
    Side s;
    s=getPlayerSide();
    int x; int y;
    Direction d;
    vector<vec2i> v;
    
    //Initialisation
    if (s==South) {
        y=_height;
        x=getPlayerPosition();
        d=Up;
    }
    else if(s==West){
        x=_width;
        y=_height - 1 - getPlayerPosition();
        d=Left;
    }
    else if (s==North){
        x =_width - 1 - getPlayerPosition();
        y = -1;
        d=Down;
    }
    else if (s==East){
        x=-1;
        y=getPlayerPosition();
        d=Right;
    }
    current_point.x = x;
    current_point.y = y;
    
    
    //Computing the next point
    
    do{
    if (d==Up) {
        
        if (_balls.find(vec2i(current_point.x,current_point.y-1))==_balls.end()){
            v.push_back(current_point);
            d=Down; //the U-turn is checked first
        }
        else if (_balls.find(vec2i(current_point.x+1,current_point.y-1))==_balls.end()){
            v.push_back(current_point);
            if (_balls.find(vec2i(current_point.x-1,current_point.y-1))==_balls.end()){
                d=Down;
            }
            else{
                d=Left;
            }
            
        }
        else if (_balls.find(vec2i(current_point.x-1,current_point.y-1))==_balls.end()){
            v.push_back(current_point);
            d=Right;
        }
        else{
            current_point.y=current_point.y+1; //if no ball is stopping you, just go for it !
        }
    }
    
    else if (d==Down){
        if (_balls.find(vec2i(current_point.x,current_point.y+1))==_balls.end()){
            v.push_back(current_point);
            d=Up;
        }
        else if (_balls.find(vec2i(current_point.x+1,current_point.y+1))==_balls.end()){
            v.push_back(current_point);
            if (_balls.find(vec2i(current_point.x-1,current_point.y+1))==_balls.end()){
                d=Up;
            }
            else{
                d=Left;
            }
            
        }
        else if (_balls.find(vec2i(current_point.x-1,current_point.y+1))==_balls.end()){
            v.push_back(current_point);
            d=Right;
        }
        else{
            current_point.y=current_point.y-1;
        }
    }
    
    else if (d==Right){
        if (_balls.find(vec2i(current_point.x+1,current_point.y))==_balls.end()){
            v.push_back(current_point);
            d=Left;
        }
        else if (_balls.find(vec2i(current_point.x+1,current_point.y+1))==_balls.end()){
            v.push_back(current_point);
            if (_balls.find(vec2i(current_point.x+1,current_point.y-1))==_balls.end()){
                d=Left;
            }
            else{
                d=Up;
            }
            
        }
        else if (_balls.find(vec2i(current_point.x+1,current_point.y-1))==_balls.end()){
            v.push_back(current_point);
            d=Down;
        }
        else{
            current_point.x=current_point.x+1;
        }
    }
    
    else if (d==Left){
        if (_balls.find(vec2i(current_point.x-1,current_point.y))==_balls.end()){
            v.push_back(current_point);
            d=Right;
        }
        else if (_balls.find(vec2i(current_point.x-1,current_point.y+1))==_balls.end()){
            v.push_back(current_point);
            if (_balls.find(vec2i(current_point.x-1,current_point.y-1))==_balls.end()){
                d=Right;
            }
            else{
                d=Up;
            }
            
        }
        else if (_balls.find(vec2i(current_point.x-1,current_point.y-1))==_balls.end()){
            v.push_back(current_point);
            d=Down;
        }
        else{
            current_point.x=current_point.x-1;
        }
    }
    }
    while (current_point.x != -1 or current_point.x != 8 or current_point.y != -1 or current_point.y != 8); //we need at least one iteration to get out of the border
    
    
    
    return v;
}