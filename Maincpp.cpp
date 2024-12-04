#include "GameObject.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include "general.h"
#include "Ladder.h"

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
int nearLadder(GameObject* player, Ladder lad[],int size, GameConfig::ARROWKEYS dir) //Checks if mario is near a ladder
{
	int distance;
	for (int i = 0;i < size;i++)
	{
		if (dir == GameConfig::UP)
		{
			if (player->getPos().getY() == lad[i].getPos().getY() && (abs(distance = (player->getPos().getX() - lad[i].getPos().getX())) <= 1))
			{
				if (distance == 1)
					player->setPos(player->getPos().getX()-1, player->getPos().getY());
				if (distance == -1)
					player->setPos(player->getPos().getX() + 1, player->getPos().getY());
				return lad[i].getSteps() + 1;
			}
				
		}
		else //Down
		{
			if (player->getPos().getY() == (lad[i].getPos().getY() - lad[i].getSteps()) && (abs(distance = (player->getPos().getX() - lad[i].getPos().getX())) <= 1))
			{
				if (distance == 1)
					player->setPos(player->getPos().getX() - 1, player->getPos().getY());
				if (distance == -1)
					player->setPos(player->getPos().getX() + 1, player->getPos().getY());
				return lad[i].getSteps() + 1;
			}
				
		}
	}

	return 0;
}


int main()
{
	GameObject mario(Point(), '@');
	GameObject pauline(Point(GameConfig::MIN_X+2,GameConfig::FLOORS::FLOOR8-1), '$');
	pauline.draw();
	drawBorders();
	drawFloors();
	Ladder l(Point(GameConfig::MIN_X + 10, GameConfig::FLOOR1 - 1),2);
	Ladder l1(Point(GameConfig::MIN_X + 30, GameConfig::FLOOR1 - 1), 2);
	Ladder arrladders[2] = { l,l1 };
	for (auto lad : arrladders)
		lad.draw();
	int wPressed = 0;
	char keyPressed = 0;
	int climb=0;
	int ladderSteps = 0;
	do {
		
		if (!wPressed)
		{
			if (_kbhit())
			{
				keyPressed = _getch();

				switch (keyPressed)
				{
				case 'a':
				case  'A':
					if(climb==0)
					mario.setDir(GameConfig::ARROWKEYS::LEFT);
					break;

				case 'd':
				case 'D':
					if(climb==0)
					mario.setDir(GameConfig::ARROWKEYS::RIGHT);
					break;

				case 'S':
				case 's':
					mario.setDir(GameConfig::ARROWKEYS::STAY);
					break;

				case 'W':
				case 'w':
				{
					GameConfig::ARROWKEYS currstate = mario.getDir();
					if (climb) // Climb Mode
					{
						if (currstate == GameConfig::ARROWKEYS::DOWN || currstate == GameConfig::ARROWKEYS::STAY) //Change Direction
						{
							climb = (ladderSteps - climb)+1;
							mario.setDir(GameConfig::ARROWKEYS::UP);
						}
							
					}
					else //Not climb mode
					{
						if (currstate ==GameConfig::STAY &&(ladderSteps=nearLadder(&mario,arrladders,2,GameConfig::UP))!=0)//Mario is near a ladder
						{
							climb = ladderSteps;
							mario.setDir(GameConfig::ARROWKEYS::UP);
						}
						else //Regular Jump
						{
							wPressed = GameConfig::JUMPSECS;
							if (currstate == GameConfig::ARROWKEYS::LEFT)
								mario.setDir(GameConfig::ARROWKEYS::UPANDLEFT);

							else if (currstate == GameConfig::ARROWKEYS::RIGHT)
								mario.setDir(GameConfig::ARROWKEYS::UPANDRIGHT);

							else //Current state is stay or down
								mario.setDir(GameConfig::ARROWKEYS::UP);
							
						}
					}
					break;
				}

				case 'X':
				case 'x':
				{

					if (climb == 0)//Checks an opportunity to tumble a ladder
					{
						if (mario.getDir() == GameConfig::STAY && (ladderSteps = nearLadder(&mario, arrladders, 2, GameConfig::DOWN)) != 0)
						{
							climb = ladderSteps;
							mario.setDir(GameConfig::ARROWKEYS::DOWN);
						}
					}
					else //On climb mode
					{
						if (mario.getDir() == GameConfig::ARROWKEYS::UP || mario.getDir() == GameConfig::ARROWKEYS::STAY)
						{
							climb = (ladderSteps - climb)+1;
							mario.setDir(GameConfig::ARROWKEYS::DOWN);
						}
							
					}
					break;
				}

				default:
					break;
				}
				
			}
			if (climb > 0) //Climb Mode
			{
				if(mario.getDir()!=GameConfig::ARROWKEYS::STAY)
				climb--;
				if (climb == 0)
					mario.setDir(GameConfig::ARROWKEYS::STAY);
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
	
		if (mario.getPos().getX() < GameConfig::MIN_X + 2)//Reached Left Bound
		{
			if (wPressed > 0)// Jump Mode
			{
				if ((mario.getDir() != GameConfig::ARROWKEYS::UP) && (mario.getDir() != GameConfig::ARROWKEYS::DOWN)) // Diagonal Jump Mode
				{
					if (wPressed - 1 >= GameConfig::JUMPSECS / 2)
						wPressed = GameConfig::JUMPSECS - wPressed;
					if (wPressed != 0)
						mario.setDir(GameConfig::ARROWKEYS::DOWN);
					else
						mario.setDir(GameConfig::ARROWKEYS::STAY);
				}
				
			}
			else if (keyPressed == 'd' || keyPressed == 'D')
				mario.setDir(GameConfig::RIGHT);
			else
			mario.setDir(GameConfig::ARROWKEYS::STAY);

		}
		if (mario.getPos().getX()  > GameConfig::MIN_X + GameConfig::WIDTH - 2)// Reached Right Bound
		{
			if (wPressed > 0)// Jump Mode
			{
				if ((mario.getDir() != GameConfig::ARROWKEYS::UP) && (mario.getDir() != GameConfig::ARROWKEYS::DOWN))// Diagonal Jump Mode
				{
					if (wPressed - 1 >= GameConfig::JUMPSECS / 2)
						wPressed = GameConfig::JUMPSECS - wPressed;
					if(wPressed!=0)
					mario.setDir(GameConfig::ARROWKEYS::DOWN);
					else
						mario.setDir(GameConfig::ARROWKEYS::STAY);
				}
			}
			else if (keyPressed == 'a' || keyPressed == 'A')
				mario.setDir(GameConfig::LEFT);
			else
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
		for (auto lad : arrladders)
			lad.draw();
		mario.draw();
		Sleep(200);
		gotoxy(mario.getPos().getX(), mario.getPos().getY());
		cout << " ";
	} while (true);
	
	gotoxy(0,GameConfig::HEIGHT + 1);
	return 0;
}
