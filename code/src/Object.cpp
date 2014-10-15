#include "Object.h"


////////////////////////////////////////////////////////////////////////////////
Object::Object(const vec2i& position_, const Direction& orientation_) :
    position(position_),
    orientation(orientation_)
{}


////////////////////////////////////////////////////////////////////////////////
void Object::turnLeft() {
    orientation = Direction(turn(orientation,-1));
}


////////////////////////////////////////////////////////////////////////////////
void Object::turnRight() {
    orientation = Direction(turn(orientation, 1));
}


////////////////////////////////////////////////////////////////////////////////
void Object::turnBack() {
    orientation = Direction(turn(orientation, 2));
}


////////////////////////////////////////////////////////////////////////////////
void Object::move()
{
    switch (orientation)
    {
        case Up: position.y -= 1; break;
        case Right: position.x += 1; break;
        case Down: position.y += 1; break;
        case Left: position.x -= 1; break;
    }
}


////////////////////////////////////////////////////////////////////////////////
void Object::left()
{
    turnLeft();
    move();
    turnRight();
}


////////////////////////////////////////////////////////////////////////////////
void Object::right()
{
    turnRight();
    move();
    turnLeft();
}


////////////////////////////////////////////////////////////////////////////////
void Object::back()
{
    turnLeft();
    turnLeft();
    move();
    turnRight();
    turnRight();
}


////////////////////////////////////////////////////////////////////////////////
int turn(const int orientation, const int nturns) {
    const int sum = orientation + nturns;
    return (sum < 0) ? (sum % 4 + 4) : (sum % 4);
};


////////////////////////////////////////////////////////////////////////////////
vec2i move(const vec2i& position, const int orientation)
{
    switch (orientation)
    {
        case Up:
            return vec2i(position.x, position.y - 1);
            break;
            
        case Right:
            return vec2i(position.x + 1, position.y);
            break;
            
        case Down:
            return vec2i(position.x, position.y + 1);
            break;
            
        case Left:
            return vec2i(position.x - 1, position.y);
            break;
            
        default:
            return position;
    }
};


////////////////////////////////////////////////////////////////////////////////
vec2i left(const vec2i& position, const int orientation) {
    return move(position, turn(orientation, -1));
};


////////////////////////////////////////////////////////////////////////////////
vec2i right(const vec2i& position, const int orientation) {
    return move(position, turn(orientation, 1));
};


////////////////////////////////////////////////////////////////////////////////
vec2i back(const vec2i& position, const int orientation) {
    return move(position, turn(orientation, 2));
};