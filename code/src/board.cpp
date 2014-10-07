#include "board.h"


////////////////////////////////////////////////////////////////////////////////
void Board::init(const int width, const int height)
{
    occupied.clear();
    
    for (int row = 0; row < height; row++) {
        occupied.push_back(std::vector<bool>(width, false));
    }
}