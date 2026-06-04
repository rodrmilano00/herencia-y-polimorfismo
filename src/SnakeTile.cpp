//
// Created by luis1 on 03/06/2026.
//

#include "SnakeTile.h"

SnakeTile::SnakeTile(int size, int penalty) : Tile('S', size), penalty(penalty) {
}

int SnakeTile::applyEffect(int pos) const {
    int result = pos - penalty;
    return result < 1 ? 1 : result;
}
