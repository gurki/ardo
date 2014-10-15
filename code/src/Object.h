#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"


////////////////////////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////////////////////////
class Object
{
    public:
    
        Object(const vec2i& position_ = vec2i(), const Direction& orientation_ = Up);
    
        void turnLeft();
        void turnRight();
        void turnBack();
        
        void move();
        void left();
        void right();
        void back();
    
        vec2i position;
        Direction orientation;
};


////////////////////////////////////////////////////////////////////////////////
int turn(const int orientation, const int nturns);

vec2i move(const vec2i& position, const int orientation);
vec2i left(const vec2i& position, const int orientation);
vec2i right(const vec2i& position, const int orientation);
vec2i back(const vec2i& position, const int orientation);


#endif
