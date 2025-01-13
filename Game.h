#pragma once
#include "Player.h"
#include "Hammer.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include "general.h"
#include "Ladder.h"
#include "Level.h"
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <map>
#include <string>
#include "FileHandler.h"

#define INTERVAL 170
#define NUMLEVELS 2
#define SECOND 1000

class Game
{
 private:
 Level* currLevel;

 public:

	 Game()
	 {
		 currLevel = new Level();
	 }

	 Level* getLevel() { return currLevel; }
	 void setLevel(Level* newlevel) { system("cls");currLevel = newlevel; }
	 void startMenu();
	 void showInstructions();
	 int lvlSelect();
	 void drawBorders();
	 void printLives(int lives, const Point& legend);
	 int showTime(const Point& legend, bool reset);
	 int getFloor(int ycoor); //an index from 0 to 7
     char getSlope(Point currpos, char board[][GameConfig::WIDTH - 2]);
	 bool LeaveLadder(const Point& currPos, const Ladder& lad, GameConfig::ARROWKEYS dir, char board[][GameConfig::WIDTH - 2]);
	 int nearLadder(Player* player, Ladder lad[], int size, GameConfig::ARROWKEYS dir, int* ladderindex, int* climb);
	 bool barrelsCheckHits(vector<Barrel>* barrels,const Player& playerPosition);
	 int barrelDistanceFloor(const Barrel& bar, int floor);
	 bool barrelsUpdateDirs(vector<Barrel>* barrels, char board[][GameConfig::WIDTH - 2], Player* mario);
	 bool marioHitsBarrel(vector<Barrel>& barrels, const Player& mario);
	 bool marioHitsGhost(vector<Ghost>& ghosts, const Player& mario);
	 void ghostsChangeDir(vector<Ghost>& ghosts);
	 bool outOfBounds(const Point& pos);
	 void printBarrelTraces(vector<Barrel> barrels);
	 void printGhostsTraces(const vector<Ghost>& ghosts);
	 void printMarioTrace(const Player& mario, const int& climb);
	 void pauseGame(const Player& mario, const vector<Barrel>& barrels, const vector<Ghost>& ghosts, const int& climb);
	 void restart(Player* mario, Point marioStartPos, vector<Barrel>* barrels, int* timetonextbarrel, int* climb, int* jumpsecs, vector<Ghost>* ghosts, const vector<Ghost>& initposesghosts);
	 void run();
};