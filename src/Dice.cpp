//
// Created by luis1 on 25/05/2026.
//

#include "Dice.h"
#include <cstdlib>
#include <ctime>
using namespace std;

//constructors

Dice::Dice(int faces) : faces(faces) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

//methods
int Dice::roll() {
    return rand() % faces + 1;
}

int Dice::getFaces() const {
    return faces;
}

Dice::~Dice() {
}
