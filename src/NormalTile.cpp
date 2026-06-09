//
// Created by luis1 on 03/06/2026.
//

#include "NormalTile.h"

NormalTile::NormalTile(int size) : Tile('N', size) {
}

int NormalTile::operator+(int pos) const {
    return pos;
}

