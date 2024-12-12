#include <iostream>
#include "general.h"
#include "Ladder.h"

using namespace std;

Ladder::Ladder(Point _startpos, int _steps) :startpos(_startpos), steps(_steps* GameConfig::FLOORDIFF)
{

}
Ladder::Ladder()
{

	startpos = Point();
	steps = 1 * GameConfig::FLOORDIFF;
}
void Ladder::draw()
{

	for (int i = this->getPos().getY();i > this->getPos().getY() - steps;i--)
	{
		gotoxy(this->getPos().getX(), i);
		cout << 'H';
	}

}