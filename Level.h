#pragma once
#include "Point.h"
#include <iostream>
#include "GameConfig.h"
#include "Ladder.h"
#include <cstring>
#include <stdlib.h>

using namespace std;

//Class and its data were given by CHATGPT
class Level {

    char board[8][GameConfig::WIDTH - 2] ;
    Ladder* ladders;
    int numLadders;

public:

    Level()
    {
        // Initialize board to 0 (no floor)
        memset(board, 0, sizeof(board));

        numLadders = 0;
        // Allocate memory for ladders
        ladders = nullptr;
    }
    ~Level();
    void setBoardValue(int row, int col, char value);
    char getBoardValue(int row, int col) const;
    void addLadder(Ladder ladder);
    Ladder getLadder(int index) const;
    Ladder* getLadders() { return ladders; }
    int getNumLadders() const;
    void printBoard() const;
    void printLadders()const;
    void initializeBoard();
    char(*getBoardPointer())[GameConfig::WIDTH - 2];
};