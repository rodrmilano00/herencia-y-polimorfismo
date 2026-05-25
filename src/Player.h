//
// Created by luis1 on 25/05/2026.
//

#ifndef DISE_OORIENTADOOBJETOS_PLAYER_H
#define DISE_OORIENTADOOBJETOS_PLAYER_H


class Player {
private:
    int id;
    int position;

public:
    //constructor
    Player(int id = 0);

    //getters and setters
    int getId() const;
    void setId(int cId);

    int getPosition() const;
    void setPosition(int cPosition);

    //destructor
    ~Player();
};


#endif //DISE_OORIENTADOOBJETOS_PLAYER_H