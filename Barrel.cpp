#include "Barrel.h"
#include <iostream>
#include "general.h"

using namespace std;
Barrel::Barrel(Point _startpos, GameConfig::ARROWKEYS _dir): pos(_startpos),dir(_dir)
{
	
}
void Barrel::draw()
{
	gotoxy(pos.getX(), pos.getY());
	cout << 'O';
}
void Barrel::move()
{
	switch (dir)
	{
	case GameConfig::UP:
		(pos.setY(pos.getY() - 1));
		break;
	case GameConfig::DOWN:
		(pos.setY(pos.getY() + 1));
		break;
	case GameConfig::RIGHT:
		(pos.setX(pos.getX() + 1));
		break;
	case GameConfig::LEFT:
		(pos.setX(pos.getX() - 1));
		break;
	case GameConfig::STAY:
		break;
	case GameConfig::DOWNANDLEFT:
		(pos.setY(pos.getY() + 1));
		(pos.setX(pos.getX() - 1));
		break;
	case GameConfig::DOWNANDRIGHT:
		(pos.setY(pos.getY() + 1));
		(pos.setX(pos.getX() + 1));
		break;
	case GameConfig::UPANDLEFT:
		(pos.setY(pos.getY() - 1));
		(pos.setX(pos.getX() - 1));
		break;
	case GameConfig::UPANDRIGHT:
		(pos.setY(pos.getY() - 1));
		(pos.setX(pos.getX() + 1));
		break;
	}
}