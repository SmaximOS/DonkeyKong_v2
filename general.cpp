#include "general.h"
#include <Windows.h>
#include <process.h>
#include <iostream>

using namespace std;
void gotoxy(int x, int y) {

    COORD coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}