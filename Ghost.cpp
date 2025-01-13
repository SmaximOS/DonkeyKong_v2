#include "Ghost.h"


Ghost::Ghost(Point _pos) : MovableObject('x', _pos, GameConfig::ARROWKEYS::STAY)
{
	//Generate rand number 0 or 1 for the starting moving direction
	char num = rand() % 2;
	num == 0 ? dir = GameConfig::ARROWKEYS::RIGHT : dir = GameConfig::ARROWKEYS::LEFT;
	pos = _pos;
	MovableObject('o', pos, dir);
}


void Ghost::move()
{
	if ((pos.getX() > GameConfig::MIN_X + GameConfig::WIDTH - 2)|| (pos.getX() < GameConfig::MIN_X + 2)) //reached the bounds 
		pos.getX() > GameConfig::MIN_X + GameConfig::WIDTH - 2 ? dir = GameConfig::ARROWKEYS::LEFT : dir = GameConfig::ARROWKEYS::RIGHT;
	else // change the direction with 0.95 chance
	{
		char num = rand() % 20;
		if (num == 0)
			dir == GameConfig::ARROWKEYS::RIGHT ? dir = GameConfig::ARROWKEYS::LEFT : dir = GameConfig::ARROWKEYS::RIGHT;
	}

	MovableObject::move();
}
