#pragma once
#include "GameConfig.h"
class Point
{
	int x;
	int y;

public:
	Point(int x = GameConfig::MIN_X + 3, int y = GameConfig::MIN_Y + GameConfig::HEIGHT - 3);
	

	int getX() { return x; };
	int getY() { return y; };
	void setX(int x) { this->x = x; };
	void setY(int y) { this->y = y; };
};

