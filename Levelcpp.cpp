#include"Level.h"
#include "general.h"

    
    

    // Destructor to free allocated memory
    Level::~Level(){
        delete[] ladders;
    }

    // Setters and Getters for the board
    void Level:: setBoardValue(int row, int col, char value) {
        if (row >= 0 && row < 8 && col >= 0 && col < GameConfig::WIDTH - 2)
            board[row][col] = value;
    }

    char Level:: getBoardValue(int row, int col) const {
        if (row >= 0 && row < 8 && col >= 0 && col < GameConfig::WIDTH - 2)
            return board[row][col];
        return -1; // Return -1 if out of bounds
    }

    // Setters and Getters for ladders
    void Level:: setLadder(int index, Ladder ladder) {
        if (index >= 0 && index < numLadders)
            ladders[index] = ladder;
    }

    Ladder Level:: getLadder(int index) const {
        if (index >= 0 && index < numLadders)
            return ladders[index];
        return Ladder(Point(), 0); // Return a default ladder if out of bounds
    }

    int Level:: getNumLadders() const {
        return numLadders;
    }

    // Method to print the board
    void Level::printBoard() const
    {
        int y = GameConfig::MIN_Y + GameConfig::HEIGHT-2;
        for (int i = 0; i < GameConfig::NUMFLOORS; ++i) 
        {
            int x = GameConfig::MIN_X;
            for (int j = 0; j < GameConfig::WIDTH - 2; ++j)
            {
                gotoxy(x, y);
               
                switch (board[i][j])
                {
                case 1:
                    cout << '=';
                case 2:
                    cout << '>';
                case 3:
                    cout << '<';
                default:
                    cout<< ' ';
                }
                x++;
            }
            y -= GameConfig::FLOORDIFF;
            cout << endl;
        }
    }


