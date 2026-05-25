//
// Created by luis1 on 25/05/2026.
//

#ifndef DISE_OORIENTADOOBJETOS_DICE_H
#define DISE_OORIENTADOOBJETOS_DICE_H


class Dice {
private:
    int faces;

public:
    //constructor
    Dice(int faces = 6);

    int roll();
    int getFaces() const;

    //destructor
    ~Dice();
};


#endif //DISE_OORIENTADOOBJETOS_DICE_H