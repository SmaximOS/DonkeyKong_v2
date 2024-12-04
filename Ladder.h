#pragma once
#include "Point.h"
class Ladder
{
	 Point startpos;
	int steps;

public:
	Ladder(Point _startpos, int _steps = 1);
	Ladder();
	void draw();
	int getSteps(){ return steps; }
	Point getPos() { return startpos; }
};