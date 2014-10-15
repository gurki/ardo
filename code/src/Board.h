#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "vector.h"
#include "Object.h"

#include <unordered_set>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
class Board
{
    public:
    
        Board(const int width, const int height, const int nballs = 0);
    
        void init(const int width, const int height);
    
        void addBall(const vec2i& pos);
        void removeBall(const vec2i& pos) { balls_.erase(pos); };
        void initBallsRandom(const int nballs);

        void addGuess(const vec2i& pos);
        void removeGuess(const vec2i& pos) { guesses_.erase(pos); };
        void clearGuesses() { guesses_.clear(); };
    
        void movePlayerRight() { playerId_++; };
        void movePlayerLeft() { playerId_--; };
        void setPlayerPosition(const int pos) { playerId_ = pos; };
    
        const int getWidth() const { return width_; };
        const int getHeight() const { return height_; };
    
        const unordered_set<vec2i>& getBalls() const { return balls_; };
        const unordered_set<vec2i>& getGuesses() const { return guesses_; };
        const size_t getNumBalls() const { return balls_.size(); };
        const size_t getNumGuesses() const { return guesses_.size(); };
    
        const bool isBall(const vec2i& pos) const;
        const bool isBorder(const vec2i& pos) const;
    
        const Side getPlayerSide() const;
        const int getPlayerPosition() const;
        const vec2i getPlayerCenter() const;
        const vec2i getPlayerEyes() const;
        const Direction getPlayerDirection() const;

        const vector<vec2i> getPath() const;
    
    
    private:
    
        unordered_set<vec2i> balls_;
        unordered_set<vec2i> guesses_;
    
        int playerId_;
        int width_;
        int height_;
};


ostream& operator << (ostream& out, const Side& side);
ostream& operator << (ostream& out, const Direction& dir);
ostream& operator << (ostream& out, const vector<vec2i>& v);


#endif
