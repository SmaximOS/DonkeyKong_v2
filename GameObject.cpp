#include "GameObject.h"
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <iostream>

using namespace std;

GameObject::GameObject(Point startpos, char representation)
{
	this->dir = GameConfig::ARROWKEYS::STAY;
	this->pos = startpos;
	this->representation = representation;
}

void GameObject::move()
{
	switch (dir)
	{
	case GameConfig::UP:
		(pos.setY(pos.getY()-1));
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
void GameObject::draw()
{
	gotoxy(pos.getX(), pos.getY());
	cout << representation;
}

