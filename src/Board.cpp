//
// Created by luis1 on 25/05/2026.
//

#include "Board.h"

//constructor
Board::Board() {
    placeTiles();
}

//snakes and ladders positions
void Board::placeTiles() {
    for (int i=0; i<size; i++) {
        tiles[i] = 'N';
    }

    //snakes
    tiles[5] = 'S'; //tile 5
    tiles[14] = 'S'; //tile 14
    tiles[22] = 'S'; //tile 22

    //ladders
    tiles[1] = 'L'; //tile 1
    tiles[10] = 'L'; //tile 10
    tiles[19] = 'L'; //tile 19
}

//methods
char Board::getTileType(int pos) const {
    return tiles[pos-1];
}

int Board::applyEffect(int pos) const {
    char type = getTileType(pos);

    if (type == 'S') {
        int result = pos-penalty;
        return result<1 ? 1 : result;
    }
    if (type == 'L') {
        int result = pos+reward;
        return result>size ? size : result;
    }
    return pos;
}

int Board::getSize() const {
    return size;
}

//destructor
Board::~Board() {
}





