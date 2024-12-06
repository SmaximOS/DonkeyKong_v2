#pragma once
#include "Point.h"
#include <iostream>
#include "GameConfig.h"
#include "Ladder.h"
#include <cstring>

using namespace std;

//Class and its data were given by CHATGPT
class Level {

    char board[8][GameConfig::WIDTH - 2] ;
    Ladder* ladders;
    int numLadders;

public:

    Level(int _numLadders) : numLadders(_numLadders)
    {
        // Initialize board to 0 (no floor)
        memset(board, 0, sizeof(board));

        // Allocate memory for ladders
        ladders = new Ladder[numLadders];
    }
    ~Level();
    void setBoardValue(int row, int col, char value);
    char getBoardValue(int row, int col) const;
    void setLadder(int index, Ladder ladder);
    Ladder getLadder(int index) const;
    int getNumLadders() const;
    void printBoard() const;
};