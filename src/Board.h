//
// Created by luis1 on 25/05/2026.
//

#ifndef DISE_OORIENTADOOBJETOS_BOARD_H
#define DISE_OORIENTADOOBJETOS_BOARD_H


class Board {
private:
    //size, penalty and reward of the board
    static const int size = 30;
    static const int penalty = 3;
    static const int reward = 3;

    //number of tiles
    char tiles[size];

    void placeTiles();

public:
    //constructor
    Board();

    //methods
    char getTileType(int pos) const;
    int applyEffect(int pos) const;
    int getSize() const;

    //destructor
    ~Board();

};


#endif //DISE_OORIENTADOOBJETOS_BOARD_H