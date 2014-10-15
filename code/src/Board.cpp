#include "Board.h"
#include "common.h"

#include <random>

    
////////////////////////////////////////////////////////////////////////////////
Board::Board(const int width,
             const int height,
             const int nballs)
{
    init(width, height);
    initBallsRandom(nballs);
    
    playerId_ = 0;
}


////////////////////////////////////////////////////////////////////////////////
void Board::init(const int width,
                 const int height)
{
    balls_.clear();
    width_ = width;
    height_ = height;
}


////////////////////////////////////////////////////////////////////////////////
void Board::initBallsRandom(const int nballs)
{
    balls_.clear();
    
    //  keep spwaning random balls until we have enough
    while (balls_.size() < nballs)
    {
        vec2i pos;
        pos.x = rand() % width_;
        pos.y = rand() % height_;
        
        //  onlpos.y add ball if position is free
        if (balls_.find(pos) == balls_.end()) {
            balls_.insert(pos);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void Board::addBall(const vec2i &pos) {
    if (!isBorder(pos)) {
        balls_.insert(pos);
    };
}


////////////////////////////////////////////////////////////////////////////////
void Board::removeBall(const vec2i &pos) {
    balls_.erase(pos);
}


////////////////////////////////////////////////////////////////////////////////
void Board::movePlayerRight() { 
    playerId_++; 
}


////////////////////////////////////////////////////////////////////////////////
void Board::movePlayerLeft() { 
    playerId_--; 
}


////////////////////////////////////////////////////////////////////////////////
void Board::shoot()
{
    /*
     TODO
     set and initiate sound moving?
     we'll see ...
     also, keep track of points and handle win/loose
     */
}


////////////////////////////////////////////////////////////////////////////////
int turn(const int dir, const int nturns) {
    const int sum = dir + nturns;
    return (sum < 0) ? (sum % 4 + 4) : (sum % 4);
}


////////////////////////////////////////////////////////////////////////////////
vec2i move(const vec2i& pos, const int dir)
{
    switch (dir)
    {
        case Board::Up:
            return vec2i(pos.x, pos.y - 1);
            break;
            
        case Board::Right:
            return vec2i(pos.x + 1, pos.y);
            break;
            
        case Board::Down:
            return vec2i(pos.x, pos.y + 1);
            break;
            
        case Board::Left:
            return vec2i(pos.x - 1, pos.y);
            break;
            
        default:
            return pos;
    }
}


////////////////////////////////////////////////////////////////////////////////
vec2i left(const vec2i& pos, const int dir) {
    return move(pos, turn(dir, -1));
}


////////////////////////////////////////////////////////////////////////////////
vec2i right(const vec2i& pos, const int dir) {
    return move(pos, turn(dir, 1));
}


////////////////////////////////////////////////////////////////////////////////
vec2i down(const vec2i& pos, const int dir) {
    return move(pos, turn(dir, 2));
}


////////////////////////////////////////////////////////////////////////////////
const Board::Side Board::getPlayerSide() const // takes the position and returns the side
{
    const int nfields = 2 * (width_ + height_);
    
    //  in case we did the full circle around the board
    int r = playerId_ % nfields;
    r = r < 0 ? r + nfields : r;

    if (r < width_) {
        return South;
    } else if (r < width_ + height_) {
        return East;
    } else if (r < 2 * width_ + height_) {
        return North;
    } else {
        return West;
    }
}


////////////////////////////////////////////////////////////////////////////////
const int Board::getPlayerPosition() const //get the position knowing the side
{
    const int nfields = 2 * (width_ + height_);
    
    int r = playerId_ % nfields;
    r = r < 0 ? r + nfields : r;

    switch (getPlayerSide())
    {
        case West:
            r -= 2 * width_ + height_;
            break;
            
        case North:
            r -= width_ + height_;
            break;
            
        case East:
            r -= width_;
            
        default:
            break;
    }
    
    return r;
}


////////////////////////////////////////////////////////////////////////////////
const vec2i Board::getPlayerCenter() const //gives the tile on which is the player
{
    const int id = getPlayerPosition();
    
    switch (getPlayerSide())
    {
        case South: return vec2i(id, height_);
        case West: return vec2i(-1, id);
        case North: return vec2i(width_ - 1 - id, -1);
        case East: return vec2i(width_, height_ - 1 - id);
    }
}


////////////////////////////////////////////////////////////////////////////////
const vec2i Board::getPlayerEyes() const { //gives where the player is looking at
    return move(getPlayerCenter(), getPlayerDirection());
}


////////////////////////////////////////////////////////////////////////////////
const Board::Direction Board::getPlayerDirection() const
{
    switch (getPlayerSide())
    {
        case South: return Up;
        case West: return Right;
        case North: return Down;
        case East: return Left;
    }
}


////////////////////////////////////////////////////////////////////////////////
const bool Board::isBall(const vec2i& pos) const {
    return balls_.find(pos) != balls_.end();
}


////////////////////////////////////////////////////////////////////////////////
const bool Board::isBorder(const vec2i& pos) const { //says if the tile is on the border
    return pos.x >= width_ || pos.x < 0 || pos.y >= height_ || pos.y < 0;
}


////////////////////////////////////////////////////////////////////////////////
const vector<vec2i> Board::getPath() const
{
    //  initialise
    int dir = getPlayerDirection();
    vec2i pos = getPlayerCenter();
    
    //  compute path
    vector<vec2i> v;
    
    do
    {
        v.push_back(pos);
        
        const vec2i step = move(pos, dir);
        
        const bool front = isBall(step);
        const bool frontLeft = isBall(left(step, dir));
        const bool frontRight = isBall(right(step, dir));
        
        //  hit
        if (front) {
            v.push_back(step);
           dir = turn(dir, 2);
        }
        //  u-turn on border
        else if (isBorder(pos) && (frontLeft || frontRight)) {
            return vector<vec2i>();
        }
        //  u-turn in field
        else if (frontLeft && frontRight) {
            dir = turn(dir, 2);
        }
        //  right deflection
        else if (frontLeft) {
            dir = turn(dir, 1);
        }
        //  left deflection
        else if (frontRight) {
            dir = turn(dir, -1);
        //  no balls, pass streight through
        } else {
            pos = step;
        }
        
        if (isBorder(pos)) {
            break;
        }
    }
    while (true); //we need at least one iteration to get out of the border
    
    return v;
}


////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& out, const Board::Side& side)
{
    switch (side)
    {
        case Board::South:
            out << "south";
            break;
            
        case Board::West:
            out << "west";
            break;
            
        case Board::North:
            out << "north";
            break;
            
        case Board::East:
            out << "east";
            break;
            
        default:
            break;
    }
    
    return out;
}


////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& out, const Board::Direction& dir)
{
    switch (dir)
    {
        case Board::Up:
            out << "up";
            break;
            
        case Board::Right:
            out << "right";
            break;
            
        case Board::Down:
            out << "down";
            break;
            
        case Board::Left:
            out << "left";
            break;
            
        default:
            break;
    }
    
    return out;
}


////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& out, const vector<vec2i>& v)
{
    out << "[";
    
    if (v.size() != 0)
    {
        for (int i = 0; i < v.size() - 1; i++) {
            out << v[i] << ", ";
        }
        
        out << v.back();
    }
    
    out << "]";
    
    return out;
}