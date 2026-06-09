//
// Created by luis1 on 03/06/2026.
//

#ifndef HERENCIAPOLIMORFISMO_TILE_H
#define HERENCIAPOLIMORFISMO_TILE_H

//abstract class
class Tile {
protected:
    char type;
    int size;

public:
    Tile(char type, int size);
    virtual ~Tile() = default;

    char getType() const;
    //operator+
    virtual int operator+(int pos) const = 0;

    friend int operator+(int pos, const Tile& t) {
        return t.operator+(pos);
    }
};


#endif //HERENCIAPOLIMORFISMO_TILE_H