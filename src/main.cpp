//
// Created by luis1 on 25/05/2026.
//

#include <iostream>
#include "Dice.h"
#include "Board.h"
#include "Player.h"
using namespace std;


class MyGame {
    private:
    //objects of game
    Board board;
    Player players[2];
    Dice dice;

    int turnCount;
    int maxTurns;
    bool gameOver;

    //input to board
    char getInput(){
        char c;
        cin >> c;
        return c;
    }

    //winner check based on position
    bool checkWinner(int pos) const {
        return pos >= board.getSize();
    }

    void printGameOver() {
        cout << "-- GAME OVER --" << endl;
    }

    void runTurn() {
        //player in turn
        int idx = (turnCount-1)%2;
        Player& p = players[idx];

        int prevPos = p.getPosition();
        int roll = dice.roll();
        int land = prevPos + roll; //dice roll and land

        if (land > board.getSize()) { //if board its exceeded
            land = board.getSize();
        }

        //applies effects and final position to player
        char tileType = board.getTileType(land);
        int finalPos = board.applyEffect(land);

        p.setPosition(finalPos);

        //format: player's turn, prevPosition, dice, tileType, finalPosition
        cout << "Turn: "<< turnCount << " "
        << "Player: " << p.getId() << " "
        << "Previous position: " << (p.getPosition() == finalPos ? land-roll : land-roll) << " "
        << "Dice: " << roll << " "
        << "Tile type: " << tileType << " "
        << "Final position: " << finalPos << endl;

        if (checkWinner(finalPos)) {
            printGameOver();
            cout << "Player " << p.getId() << " is the winner!!!" << endl;
            gameOver = true;
        }
    }

public:
    //game constructor
    MyGame() :players{Player(1), Player(2)}, turnCount(0), maxTurns(100), gameOver(false) {}

    void start() {
        cout << "Press C to continue next turn, or E to end the game:" << endl;

        char input;
        while (!gameOver) {
            input = getInput();

            if (input == 'E') {
                cout << "Thanks for playing!!!" << endl;
                gameOver = true;
            }
            else if (input != 'C') {
                cout << "Invalid option, please press C to continue next turn, or E to end the game:" << endl;
                continue;
            }

            turnCount++;
            runTurn();

            if (gameOver) return;

            if (turnCount >= maxTurns) {
                printGameOver();
                cout << "The maximum number of turns has been reached..." << endl;
                gameOver = true;
            }
        }
    }
};

int main() {
    MyGame g;
    g.start();
    return 0;
}