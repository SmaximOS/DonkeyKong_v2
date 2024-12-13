#include"Level.h"
#include "general.h"




// Destructor to free allocated memory
Level::~Level() {
    delete[] ladders;
}

// Setters and Getters for the board
void Level::setBoardValue(int row, int col, char value) {
    if (row >= 0 && row < 8 && col >= 0 && col < GameConfig::WIDTH - 2)
        board[row][col] = value;
}

char Level::getBoardValue(int row, int col) const {
    if (row >= 0 && row < 8 && col >= 0 && col < GameConfig::WIDTH - 2)
        return board[row][col];
    return -1; // Return -1 if out of bounds
}

// Setters and Getters for ladders
void Level::addLadder(Ladder ladder)
{
    numLadders++;
    if (numLadders == 1)
        ladders = (Ladder*)malloc(sizeof(Ladder));
    else
        ladders = (Ladder*)realloc(ladders, sizeof(Ladder) * numLadders);

    ladders[numLadders - 1] = ladder;
}

Ladder Level::getLadder(int index) const {
    if (index >= 0 && index < numLadders)
        return ladders[index];
    return Ladder(Point(), 0); // Return a default ladder if out of bounds
}

int Level::getNumLadders() const {
    return numLadders;
}
char(*Level::getBoardPointer())[GameConfig::WIDTH - 2] {
    return board;
    }

    // Method to print the board
        void Level::printBoard() const
    {
        int y = GameConfig::FLOOR1;
        for (int i = 0; i < GameConfig::NUMFLOORS; i++)
        {
            int x = GameConfig::MIN_X + 1; // Start inside the border
            for (int j = 0; j < GameConfig::WIDTH - 2; j++) // Full width now
            {
                gotoxy(x, y);
                switch (board[i][j])
                {
                case 1:
                    cout << '=';
                    break;
                case 2:
                    cout << '>';
                    break;
                case 3:
                    cout << '<';
                    break;
                case 0:
                    cout << ' ';
                    break;
                }
                x++;
            }
            gotoxy(x, y);
            switch (board[i][GameConfig::WIDTH - 3])
            {
            case 1:
                cout << '=';
                break;
            case 2:
                cout << '>';
                break;
            case 3:
                cout << '<';
                break;
            case 0:
                cout << ' ';
                break;
            }
            y -= GameConfig::FLOORDIFF;
        }
        printLadders();
    }
    void Level::printLadders()const
    {

        for (int i = 0;i < numLadders;i++)
        {

            ladders[i].draw();
        }
    }
    void Level::initializeBoard1()
    {
        int i;

        //Floor 1
        for (i = 0;i <= 30;i++)
            board[0][i] = 1;

        board[0][30] = 2;//Right Slope

        for (i = 71;i < GameConfig::WIDTH - 2;i++)
            board[0][i] = 1;
        board[0][71] = 3;

        //Floor2
        for (i = 0;i <= 15;i++)
            board[1][i] = 1;
        board[1][15] = 2;

        for (i = 27;i <= 74;i++)
            board[1][i] = 1;
        board[1][74] = 2;

        for (i = 78;i < GameConfig::WIDTH - 2;i++)
            board[1][i] = 1;

        //Floor3
        for (i = 4;i <= 19;i++)
            board[2][i] = 1;
        board[2][19] = 2;

        for (i = 22;i <= 50;i++)
            board[2][i] = 1;
        board[2][50] = 2;

        for (i = 67;i <= 80;i++)
            board[2][i] = 1;
        for (i = 85;i < GameConfig::WIDTH - 2;i++)
            board[2][i] = 1;

        //Floor4
        for (i = 12;i <= 31;i++)
            board[3][i] = 1;
        board[3][31] = 2;

        for (i = 34;i <= 43;i++)
            board[3][i] = 1;
        board[3][43] = 2;

        for (i = 55;i <= 100;i++)
            board[3][i] = 1;
        board[3][55] = 3;

        //Floor5
        for (i = 33;i <= 70;i++)
            board[4][i] = 1;
        board[4][33] = 3;

        //Floor6
        for (i = 0;i <= 16;i++)
            board[5][i] = 1;
        for (i = 26;i <= 66;i++)
            board[5][i] = 1;
        board[5][26] = 3;
        for (i = 83;i < GameConfig::WIDTH - 2;i++)
            board[5][i] = 1;
        board[5][83] = 3;

        //Floor7
        for (i = 4;i <= 19;i++)
            board[6][i] = 1;
        for (i = 22;i <= 34;i++)
            board[6][i] = 1;
        board[6][34] = 2;
        for (i = 47;i <= 60;i++)
            board[6][i] = 1;
        for (i = 63;i <= 73;i++)
            board[6][i] = 1;
        board[6][73] = 2;
        for (i = 82;i <= 98;i++)
            board[6][i] = 1;

        //Floor8
        for (i = 0;i <= 30;i++)
            board[7][i] = 1;
        for (i = 50;i <= 58;i++)
            board[7][i] = 1;

        //Ladders

        addLadder(Ladder(Point(GameConfig::MIN_X + 10, GameConfig::FLOOR1 - 1), 5));
        addLadder(Ladder(Point(GameConfig::MIN_X + 30, GameConfig::FLOOR1 - 1), 2));
        addLadder(Ladder(Point(GameConfig::MIN_X + 73, GameConfig::FLOOR1 - 1), 2));
        addLadder(Ladder(Point(GameConfig::MIN_X + 92, GameConfig::FLOOR2 - 1), 2));
        addLadder(Ladder(Point(GameConfig::MIN_X + 60, GameConfig::FLOOR4 - 1), 2));
        addLadder(Ladder(Point(GameConfig::MIN_X + 28, GameConfig::FLOOR6 - 1), 2));
        addLadder(Ladder(Point(GameConfig::MIN_X + 97, GameConfig::FLOOR2 - 1), 5));
        addLadder(Ladder(Point(GameConfig::MIN_X + 55, GameConfig::FLOOR6 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 37, GameConfig::FLOOR4 - 1), 1));

        

        //Characters starting Positions
        setstartPosMario(Point());  //Default Position
        setstartPosPauline(Point(GameConfig::MIN_X + 2, GameConfig::FLOORS::FLOOR8 - 1));
        setstartPosDonkeyKong(Point(GameConfig::MIN_X + 54, GameConfig::FLOORS::FLOOR8 - 1));

        //Barrels Settings
        barrelsSets.size = 4; //indicates that throwing barrels will repeat itself every 4 barrels
        barrelsSets.intervalsBetweenBarrels = new int[4];
        barrelsSets.intervalsBetweenBarrels[0] = 9; //Amount of time until first barrel, fifth barrel..
        barrelsSets.intervalsBetweenBarrels[1] = 25; //Amount of time until second barrel, sixth barrel..
        barrelsSets.intervalsBetweenBarrels[2] = 25;
        barrelsSets.intervalsBetweenBarrels[3] = 25;
        barrelsSets.dirs = new GameConfig::ARROWKEYS[4];
        barrelsSets.dirs[0] = GameConfig::ARROWKEYS::RIGHT; //Starting direction of first,fifth.. barrels
        barrelsSets.dirs[1] = GameConfig::ARROWKEYS::LEFT;
        barrelsSets.dirs[2] = GameConfig::ARROWKEYS::LEFT;
        barrelsSets.dirs[3] = GameConfig::ARROWKEYS::RIGHT;

    }
    void Level::initializeBoard2()
    {
        int i;

        //Floor 1
        for (i = 0;i <= 28;i++)
            board[0][i] = 1;

        board[0][0] = 3;//Right Slope

        for (i = 31; i <= 38; i++)
            board[0][i] = 1;
        board[0][31] = 3;

        for (i = 41;i < 52;i++)
            board[0][i] = 1;
        board[0][52] = 2;

        //Floor2
        for (i = 10;i <= 32;i++)
            board[1][i] = 1;
        board[1][32] = 2;

        for (i = 47;i <= 59;i++)
            board[1][i] = 1;
        board[1][47] = 3;

        for (i = 62;i < 90;i++)
            board[1][i] = 1;
        board[1][90] = 2;

        //Floor3
        for (i = 0;i <= 21;i++)
            board[2][i] = 1;
        board[2][21] = 2;

        for (i = 28; i <= 36; i++)
            board[2][i] = 1;
        board[2][28] = 3;

        for (i = 46; i <= 50; i++)
            board[2][i] = 1;
        board[2][50] = 2;

        for (i = 53; i <= 57; i++)
            board[2][i] = 1;
        board[2][53] = 3;

        for (i = 61; i <= 65; i++)
            board[2][i] = 1;
        board[2][65] = 2;

        for (i = 68; i <= 72; i++)
            board[2][i] = 1;
        board[2][68] = 3;

        for (i = 80;i < GameConfig::WIDTH - 2;i++)
            board[2][i] = 1;
        board[2][80] = 3;

        //Floor4
        for (i = 3;i <= 26;i++)
            board[3][i] = 1;
        board[3][3] = 3;

        for (i = 32;i <= 37;i++)
            board[3][i] = 1;
        board[3][32] = 3;

        for (i = 70;i <= 85;i++)
            board[3][i] = 1;
        board[3][70] = 3;

        //Floor5
        for (i = 30; i <= 39; i++)
            board[4][i] = 1;
        board[4][39] = 2;

        for (i = 15; i <= 27; i++)
            board[4][i] = 1;
        board[4][15] = 3;

        for (i = 2; i <= 12; i++)
            board[4][i] = 1;
        board[4][12] = 2;

        //Floor6
        for (i = 0; i <= 16; i++)
            board[5][i] = 1;
        board[5][16] = 2;

        for (i = 18; i <= 22; i++)
            board[5][i] = 1;
        board[5][18] = 3;

        for (i = 24; i <= 28; i++)
            board[5][i] = 1;
        board[5][24] = 3;

        for (i = 30; i <= 76; i++)
            board[5][i] = 1;
        board[5][76] = 2;

        board[5][41] = 0;

        //Floor7
        for (i = 4;i <= 19;i++)
            board[6][i] = 1;
        board[6][4] = 3;

        for (i = 22;i <= 34;i++)
            board[6][i] = 1;
        board[6][34] = 2;

        for (i = 37; i <= 44; i++)
            board[6][i] = 1;
        board[6][37] = 3;

        for (i = 47;i <= 60;i++)
            board[6][i] = 1;
        board[6][60] = 2;

        for (i = 63;i <= 73;i++)
            board[6][i] = 1;
        board[6][73] = 2;


        //Floor8
        for (i = 0;i <= 30;i++)
            board[7][i] = 1;
        for (i = 50;i <= 57;i++)
            board[7][i] = 1;

        //Ladders

        addLadder(Ladder(Point(GameConfig::MIN_X + 12, GameConfig::FLOOR1 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 50, GameConfig::FLOOR1 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 89, GameConfig::FLOOR2 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 15, GameConfig::FLOOR2 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 6, GameConfig::FLOOR3 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 34, GameConfig::FLOOR3 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 83, GameConfig::FLOOR3 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 47, GameConfig::FLOOR3 - 1), 3));
        addLadder(Ladder(Point(GameConfig::MIN_X + 73, GameConfig::FLOOR3 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 5, GameConfig::FLOOR5 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 37, GameConfig::FLOOR4 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 72, GameConfig::FLOOR6 - 1), 1));
        addLadder(Ladder(Point(GameConfig::MIN_X + 10, GameConfig::FLOOR7 - 1), 1));


        //Characters starting Positions
        setstartPosMario(Point());  //Default Position
        setstartPosPauline(Point(GameConfig::MIN_X + 2, GameConfig::FLOORS::FLOOR8 - 1));
        setstartPosDonkeyKong(Point(GameConfig::MIN_X + 54, GameConfig::FLOORS::FLOOR8 - 1));

        //Barrels Settings
        barrelsSets.size = 4; //indicates that throwing barrels will repeat itself every 4 barrels
        barrelsSets.intervalsBetweenBarrels = new int[5];
        barrelsSets.intervalsBetweenBarrels[0] = 9; //Amount of time until first barrel, fifth barrel..
        barrelsSets.intervalsBetweenBarrels[1] = 25; //Amount of time until second barrel, sixth barrel..
        barrelsSets.intervalsBetweenBarrels[2] = 25;
        barrelsSets.intervalsBetweenBarrels[3] = 16;
        barrelsSets.intervalsBetweenBarrels[4] = 22;
        barrelsSets.dirs = new GameConfig::ARROWKEYS[5];
        barrelsSets.dirs[0] = GameConfig::ARROWKEYS::RIGHT; //Starting direction of first,fifth.. barrels
        barrelsSets.dirs[1] = GameConfig::ARROWKEYS::LEFT;
        barrelsSets.dirs[2] = GameConfig::ARROWKEYS::LEFT;
        barrelsSets.dirs[3] = GameConfig::ARROWKEYS::RIGHT;
        barrelsSets.dirs[3] = GameConfig::ARROWKEYS::LEFT;

    }


