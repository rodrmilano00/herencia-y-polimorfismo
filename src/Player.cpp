//
// Created by luis1 on 25/05/2026.
//

#include "Player.h"

//constructor
Player::Player(int id) : id(id), position(1) {}

//getters and setters
int Player::getId() const {
    return this->id;
}
int Player::getPosition() const {
    return this->position;
}

void Player::setId(int cId) {
    this->id = cId;
}
void Player::setPosition(int cPosition) {
    this->position = cPosition;
}

//destructor
Player::~Player() {
}


