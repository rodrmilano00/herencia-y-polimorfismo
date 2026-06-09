//
// Created by luis1 on 03/06/2026.
//

#include "LadderTile.h"

LadderTile::LadderTile(int size, int reward) : Tile('L', size), reward(reward) {
}

int LadderTile::operator+(int pos) const {
    int result = pos + reward;
    return result > size ? size : result;
}


