#ifndef BOARD_H
#define BOARD_H

#include <vector>


class Board
{
    public:
    
        Board() {};
    
        void init(const int width, const int height);
    
    
    private:
    
        std::vector< std::vector<bool> > occupied;
};


#endif
