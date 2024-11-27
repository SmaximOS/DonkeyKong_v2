#include "GameObject.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include "general.h"
using namespace std;

void drawBorders() 
{
	

	// Top and bottom borders
	for (int x = GameConfig::MIN_X; x < GameConfig::MIN_X + GameConfig::WIDTH; ++x)
	{
		gotoxy(x, GameConfig::MIN_Y);
		cout << "Q"; // Top border
		gotoxy(x, GameConfig::MIN_Y + GameConfig::HEIGHT-1);
		cout << "Q"; // Bottom border
	}

	// Left and right borders
	for (int y = GameConfig::MIN_Y; y < GameConfig::MIN_Y + GameConfig::HEIGHT; ++y) {
		gotoxy(GameConfig::MIN_X, y);
		cout << "Q"; // Left border
		gotoxy(GameConfig::MIN_X + GameConfig::WIDTH, y);
		cout << "Q"; // Right border
	}
}

void drawFloors()
{
	for (int i = GameConfig::FLOORS::FLOOR1;i >= GameConfig::FLOORS::FLOOR8;i -= GameConfig::FLOORDIFF)
	{
		for (int j = GameConfig::MIN_X + 1;j <= GameConfig::MIN_X + GameConfig::WIDTH - 1;j++)
		{
			gotoxy(j, i);
			cout << "=";
		}
	}
}
	


int main()
{
	Point p=Point();
	GameObject mario(p, '@');
	drawBorders();

	drawFloors();
	int wPressed = 0;
	do {
		
		if (!wPressed)
		{
			if (_kbhit())
			{
				char keyPressed = _getch();

				switch (keyPressed)
				{
				case 'a':
				case  'A':
					mario.setDir(GameConfig::ARROWKEYS::LEFT);
					break;

				case 'd':
				case 'D':
					mario.setDir(GameConfig::ARROWKEYS::RIGHT);
					break;

				case 'S':
				case 's':
					mario.setDir(GameConfig::ARROWKEYS::STAY);
					break;

				case 'W':
				case 'w':
				{
					wPressed = GameConfig::JUMPSECS;
					GameConfig::ARROWKEYS currstate = mario.getDir();

					if (currstate == GameConfig::ARROWKEYS::LEFT)
						mario.setDir(GameConfig::ARROWKEYS::UPANDLEFT);

					else if (currstate == GameConfig::ARROWKEYS::RIGHT)
						mario.setDir(GameConfig::ARROWKEYS::UPANDRIGHT);

					else //Current state is stay or down
						mario.setDir(GameConfig::ARROWKEYS::UP);
					break;
				}

				default:
					break;
				}
			}

		}
		else
		{
			if (wPressed-1 == GameConfig::JUMPSECS / 2)
			{
				
				GameConfig::ARROWKEYS currstate = mario.getDir();
				
				if (currstate == GameConfig::UPANDLEFT)
					mario.setDir(GameConfig::ARROWKEYS::DOWNANDLEFT);
				else if (currstate == GameConfig::UPANDRIGHT)
				{
					mario.setDir(GameConfig::ARROWKEYS::DOWNANDRIGHT);
				}
					
				else // Current State is Up
					mario.setDir(GameConfig::ARROWKEYS::DOWN);
			}
				wPressed--;
				if (wPressed == 0)
				{
					GameConfig::ARROWKEYS currstate = mario.getDir();
					if (currstate == GameConfig::ARROWKEYS::DOWNANDLEFT)
						mario.setDir(GameConfig::ARROWKEYS::LEFT);
					else if (currstate == GameConfig::DOWNANDRIGHT)
						mario.setDir(GameConfig::ARROWKEYS::RIGHT);
					else // Current State is Down
						mario.setDir(GameConfig::ARROWKEYS::STAY);
				}

		}
	
		if (mario.getPos().getX() < GameConfig::MIN_X + 2)
		{
			mario.setDir(GameConfig::ARROWKEYS::STAY);
		}
		if (mario.getPos().getX()  > GameConfig::MIN_X + GameConfig::WIDTH - 2)
		{
			mario.setDir(GameConfig::ARROWKEYS::STAY);
		}
		if (mario.getPos().getY() < GameConfig::MIN_Y + 1)
		{
			break;
		}
		if (mario.getPos().getY() > GameConfig::MIN_Y + GameConfig::HEIGHT - 1)
		{
			break;
		}

		mario.move();
		mario.draw();
		Sleep(350);
		gotoxy(mario.getPos().getX(), mario.getPos().getY());
		cout << " ";
	} while (true);
	
	gotoxy(0,GameConfig::HEIGHT + 1);
	return 0;
}
