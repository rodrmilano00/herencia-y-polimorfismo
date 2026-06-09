//
// Created by luis1 on 03/06/2026.
//

#ifndef HERENCIAPOLIMORFISMO_SNAKETILE_H
#define HERENCIAPOLIMORFISMO_SNAKETILE_H
#include "Tile.h"
#include <cmath>

class SnakeTile : public Tile {
private:
    int penalty;

public:
    SnakeTile(int size, int penalty);
    int operator+(int pos) const override;
};


#endif //HERENCIAPOLIMORFISMO_SNAKETILE_H