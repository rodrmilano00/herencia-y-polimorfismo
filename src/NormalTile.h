//
// Created by luis1 on 03/06/2026.
//

#ifndef HERENCIAPOLIMORFISMO_NORMALTILE_H
#define HERENCIAPOLIMORFISMO_NORMALTILE_H
#include "Tile.h"


class NormalTile : public Tile {
public:
    explicit NormalTile(int size);
    int applyEffect(int pos) const override;
};


#endif //HERENCIAPOLIMORFISMO_NORMALTILE_H