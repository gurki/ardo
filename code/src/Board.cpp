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
    }
}


////////////////////////////////////////////////////////////////////////////////
void Board::addGuess(const vec2i &pos)
{
    //  only add guess if
    //      - the position is on the board
    //      - there are balls to guess left
    if (!isBorder(pos) && guesses_.size() < balls_.size()) {
        guesses_.insert(pos);
    }
}


////////////////////////////////////////////////////////////////////////////////
const Side Board::getPlayerSide() const // takes the position and returns the side
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
const Direction Board::getPlayerDirection() const
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
const vector<vec2i> Board::getPath(const bool full) const
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
            if (!isBorder(pos) && !full) v.pop_back();
            v.push_back(step);
            return v;
        }
        //  u-turn on border
        else if (isBorder(pos) && (frontLeft || frontRight)) {
            v.push_back(step);
            return v;
        }
        //  u-turn in field
        else if (frontLeft && frontRight) {
            if (!full) v.pop_back();
            v.push_back(step);
            dir = turn(dir, 2);
            pos = move(pos, dir);
        }
        //  right deflection
        else if (frontLeft) {
            dir = turn(dir, 1);
            pos = move(pos, dir);
        }
        //  left deflection
        else if (frontRight) {
            dir = turn(dir, -1);
            pos = move(pos, dir);
        //  no balls, pass streight through
        } else {
            if (!isBorder(pos) && !full) v.pop_back();
            pos = step;
        }
        
        if (isBorder(pos)) {
            v.push_back(pos);
            break;
        }
    }
    while (true); //we need at least one iteration to get out of the border
    
    return v;
}


////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& out, const Side& side)
{
    switch (side)
    {
        case South:
            out << "south";
            break;
            
        case West:
            out << "west";
            break;
            
        case North:
            out << "north";
            break;
            
        case East:
            out << "east";
            break;
            
        default:
            break;
    }
    
    return out;
}


////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& out, const Direction& dir)
{
    switch (dir)
    {
        case Up:
            out << "up";
            break;
            
        case Right:
            out << "right";
            break;
            
        case Down:
            out << "down";
            break;
            
        case Left:
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