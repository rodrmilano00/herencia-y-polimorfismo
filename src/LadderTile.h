//
// Created by luis1 on 03/06/2026.
//

#ifndef HERENCIAPOLIMORFISMO_LADDERTILE_H
#define HERENCIAPOLIMORFISMO_LADDERTILE_H
#include "Tile.h"


class LadderTile : public Tile {
private:
    int reward;

public:
    LadderTile(int size, int reward);
    int applyEffect(int pos) const override;
};


#endif //HERENCIAPOLIMORFISMO_LADDERTILE_H