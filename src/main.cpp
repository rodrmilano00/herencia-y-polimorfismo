//
// Created by luis1 on 25/05/2026.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "dice.h"
#include "player.h"
#include "tile.h"
#include "NormalTile.h"
#include "SnakeTile.h"
#include "LadderTile.h"


//Clase Turn + operator
struct Turn {
    int  turnNum;
    int  playerId;
    int  prevPos;
    int  roll;
    char tileType;
    int  finalPos;

    friend std::ostream& operator<<(std::ostream& os, const Turn& t) {
        os << t.turnNum  << " "
           << t.playerId << " "
           << t.prevPos  << " "
           << t.roll     << " "
           << t.tileType << " "
           << t.finalPos;
        return os;
    }
};

//MyGame (abstracta)
class MyGame {
protected:
    int     numTiles;
    int     numSnakes;
    int     numLadders;
    int     penalty;
    int     reward;
    int     numPlayers;
    int     maxTurns;

    Tile**  tiles;
    Player* players;
    Dice    dice;

    int  turnCount;
    bool gameOver;

    void buildBoard() {
        for (int i = 0; i < numTiles; i++)
            tiles[i] = new NormalTile(numTiles);

        srand(static_cast<unsigned int>(time(nullptr)));

        int placed = 0;
        while (placed < numSnakes) {
            int pos = rand() % (numTiles - 2) + 1;
            if (tiles[pos]->getType() == 'N') {
                delete tiles[pos];
                tiles[pos] = new SnakeTile(numTiles, penalty);
                placed++;
            }
        }

        placed = 0;
        while (placed < numLadders) {
            int pos = rand() % (numTiles - 2) + 1;
            if (tiles[pos]->getType() == 'N') {
                delete tiles[pos];
                tiles[pos] = new LadderTile(numTiles, reward);
                placed++;
            }
        }
    }

    bool checkWinner(int pos) const {
        return pos >= numTiles;
    }

    void printGameOver() const {
        std::cout << "-- GAME OVER --" << std::endl;
    }

public:
    MyGame(int t, int s, int l, int pen, int rew, int p, int mt)
        : numTiles(t), numSnakes(s), numLadders(l),
          penalty(pen), reward(rew), numPlayers(p),
          maxTurns(mt), turnCount(0), gameOver(false)
    {
        players = new Player[numPlayers];
        for (int i = 0; i < numPlayers; i++)
            players[i] = Player(i + 1);

        tiles = new Tile*[numTiles];
        buildBoard();
    }

    virtual ~MyGame() {
        for (int i = 0; i < numTiles; i++)
            delete tiles[i];
        delete[] tiles;
        delete[] players;
    }

    virtual void start()   = 0;
    virtual void runTurn() = 0;
};

//ManualGame
class ManualGame : public MyGame {
private:
    char getInput() {
        char c;
        std::cin >> c;
        std::cin.ignore();
        return c;
    }

    void executeTurn() {
        int idx     = (turnCount - 1) % numPlayers;
        Player& p   = players[idx];

        int prevPos = p.getPosition();
        int roll    = dice.roll();
        int landed  = prevPos + roll;

        if (landed > numTiles)
            landed = numTiles;

        // operator+ : landed + tile resuelve la posición final
        int finalPos = landed + *tiles[landed - 1];

        p.setPosition(finalPos);

        // operator<< imprime el turno
        Turn t { turnCount, p.getId(), prevPos, roll,
                 tiles[landed - 1]->getType(), finalPos };
        std::cout << t << std::endl;

        if (checkWinner(finalPos)) {
            printGameOver();
            std::cout << "Player " << p.getId() << " is the winner!!!" << std::endl;
            gameOver = true;
        }
    }

public:
    ManualGame(int t, int s, int l, int pen, int rew, int p, int mt)
        : MyGame(t, s, l, pen, rew, p, mt) {}

    void runTurn() override { executeTurn(); }

    void start() override {
        std::cout << "Press C to continue next turn, or E to end the game:" << std::endl;

        while (!gameOver) {
            char input = getInput();

            if (input == 'E') {
                std::cout << "Thanks for playing!!!" << std::endl;
                return;
            }
            if (input != 'C') {
                std::cout << "Invalid option, please press C to continue next turn or E to end the game" << std::endl;
                continue;
            }

            turnCount++;
            runTurn();
            if (gameOver) return;

            if (turnCount >= maxTurns) {
                printGameOver();
                std::cout << "The maximum number of turns has been reached..." << std::endl;
                return;
            }
        }
    }
};

//AutoGame
class AutoGame : public MyGame {
private:
    void executeTurn() {
        int idx     = (turnCount - 1) % numPlayers;
        Player& p   = players[idx];

        int prevPos = p.getPosition();
        int roll    = dice.roll();
        int landed  = prevPos + roll;

        if (landed > numTiles)
            landed = numTiles;

        int finalPos = landed + *tiles[landed - 1];

        p.setPosition(finalPos);

        Turn t { turnCount, p.getId(), prevPos, roll,
                 tiles[landed - 1]->getType(), finalPos };
        std::cout << t << std::endl;

        if (checkWinner(finalPos)) {
            printGameOver();
            std::cout << "Player " << p.getId() << " is the winner!!!" << std::endl;
            gameOver = true;
        }
    }

public:
    AutoGame(int t, int s, int l, int pen, int rew, int p, int mt)
        : MyGame(t, s, l, pen, rew, p, mt) {}

    void runTurn() override { executeTurn(); }

    void start() override {
        while (!gameOver) {
            turnCount++;
            runTurn();
            if (gameOver) return;

            if (turnCount >= maxTurns) {
                printGameOver();
                std::cout << "The maximum number of turns has been reached..." << std::endl;
                return;
            }
        }
    }
};

//main
int main(int argc, char* argv[]) {
    int  t   = 30, s  = 3, l  = 3;
    int  pen = 3,  rew = 3, p  = 2;
    int  mt  = 100;
    char type = 'M';

    if (argc == 9) {
        t    = std::atoi(argv[1]);
        s    = std::atoi(argv[2]);
        l    = std::atoi(argv[3]);
        pen  = std::atoi(argv[4]);
        rew  = std::atoi(argv[5]);
        p    = std::atoi(argv[6]);
        mt   = std::atoi(argv[7]);
        type = argv[8][0];
    }

    MyGame* game = (type == 'A')
        ? static_cast<MyGame*>(new AutoGame(t, s, l, pen, rew, p, mt))
        : static_cast<MyGame*>(new ManualGame(t, s, l, pen, rew, p, mt));

    game->start();
    delete game;
    return 0;
}