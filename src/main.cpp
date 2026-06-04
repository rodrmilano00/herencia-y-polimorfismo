//
// Created by luis1 on 25/05/2026.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "dice.h"
#include "player.h"
#include "tile.h"
#include "NormalTile.h"
#include "SnakeTile.h"
#include "LadderTile.h"

class MyGame {
protected:
    int numTiles;
    int numSnakes;
    int numLadders;
    int penalty;
    int reward;
    int numPlayers;
    int maxTurns;

    Tile**  tiles;
    Player* players;
    Dice dice;

    int turnCount;
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

    void printTurn(int turn, int playerId, int prevPos,
                   int roll, char tileType, int finalPos) const {
        std::cout << turn     << " "
                  << playerId << " "
                  << prevPos  << " "
                  << roll     << " "
                  << tileType << " "
                  << finalPos << std::endl;
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

// manual game class
class ManualGame : public MyGame {
private:
    char getInput() {
        char c;
        std::cin >> c;
        std::cin.ignore();
        return c;
    }

public:
    ManualGame(int t, int s, int l, int pen, int rew, int p, int mt)
        : MyGame(t, s, l, pen, rew, p, mt) {}

    void runTurn() override {
        int idx     = (turnCount - 1) % numPlayers;
        Player& p   = players[idx];

        int prevPos = p.getPosition();
        int roll    = dice.roll();
        int landed  = prevPos + roll;

        if (landed > numTiles)
            landed = numTiles;

        char tileType = tiles[landed - 1]->getType();
        int  finalPos = tiles[landed - 1]->applyEffect(landed);

        p.setPosition(finalPos);
        printTurn(turnCount, p.getId(), prevPos, roll, tileType, finalPos);

        if (checkWinner(finalPos)) {
            printGameOver();
            std::cout << "Player " << p.getId() << " is the winner!!!" << std::endl;
            gameOver = true;
        }
    }

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

// auto game class
class AutoGame : public MyGame {
public:
    AutoGame(int t, int s, int l, int pen, int rew, int p, int mt)
        : MyGame(t, s, l, pen, rew, p, mt) {}

    void runTurn() override {
        int idx     = (turnCount - 1) % numPlayers;
        Player& p   = players[idx];

        int prevPos = p.getPosition();
        int roll    = dice.roll();
        int landed  = prevPos + roll;

        if (landed > numTiles)
            landed = numTiles;

        char tileType = tiles[landed - 1]->getType();
        int  finalPos = tiles[landed - 1]->applyEffect(landed);

        p.setPosition(finalPos);
        printTurn(turnCount, p.getId(), prevPos, roll, tileType, finalPos);

        if (checkWinner(finalPos)) {
            printGameOver();
            std::cout << "Player " << p.getId() << " is the winner!!!" << std::endl;
            gameOver = true;
        }
    }

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

// main
int main(int argc, char* argv[]) {
    int  tiles   = 30;
    int  snakes  = 3;
    int  ladders = 3;
    int  penalty = 3;
    int  reward  = 3;
    int  players = 2;
    int  turns   = 100;
    char type    = 'M';

    if (argc == 9) {
        tiles   = std::atoi(argv[1]);
        snakes  = std::atoi(argv[2]);
        ladders = std::atoi(argv[3]);
        penalty = std::atoi(argv[4]);
        reward  = std::atoi(argv[5]);
        players = std::atoi(argv[6]);
        turns   = std::atoi(argv[7]);
        type    = argv[8][0];
    }

    MyGame* game = nullptr;

    if (type == 'A')
        game = new AutoGame(tiles, snakes, ladders, penalty, reward, players, turns);
    else
        game = new ManualGame(tiles, snakes, ladders, penalty, reward, players, turns);

    game->start();
    delete game;
    return 0;
}