#pragma once
#include <iostream>
#include "Point.h"
#include "GameConfig.h"

using namespace std;

class Barrel
{
	Point pos;
	GameConfig::ARROWKEYS dir;
	

public:
	Barrel(Point _startpos,GameConfig::ARROWKEYS _dir);
	void draw();
	void move();
	void setPos(int x, int y) { pos.setX(x);pos.setY(y); }
	Point getPos() { return pos; }
	void setDir(GameConfig::ARROWKEYS newdir) { dir = newdir; }
	GameConfig::ARROWKEYS getDir(){ return dir; }
	
};
	
typedef struct  LEVELSETTINGS
{
	int size; //The size of the barrels to repeat  
	int* intervalsBetweenBarrels; //the intervals between each barrel (size)
	GameConfig::ARROWKEYS* dirs; // the initial direction of each barrel

} LevelSettings;