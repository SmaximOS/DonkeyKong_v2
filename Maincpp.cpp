#include "GameObject.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include "general.h"
#include "Ladder.h"
#include "Level.h"

using namespace std;

void drawBorders() 
{

	// Top and bottom borders
	for (int x = GameConfig::MIN_X; x < GameConfig::MIN_X + GameConfig::WIDTH; ++x)
	{
		gotoxy(x, GameConfig::MIN_Y);
		cout << "Q"; // Top border
		gotoxy(x, GameConfig::MIN_Y + GameConfig::HEIGHT);
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
bool LeaveLadder(int ypos, Ladder l)// Checks if there is an option to leave the ladder int the middle
{
	
	for (int middlefloor = l.getPos().getY() - GameConfig::FLOORDIFF; middlefloor > l.getPos().getY() - l.getSteps() ; middlefloor -= GameConfig::FLOORDIFF)
	{
		if (ypos == middlefloor)
			return true;
	}
	return false;
}

int nearLadder(GameObject* player, Ladder lad[],int size, GameConfig::ARROWKEYS dir,int* ladderindex,int* climb) //Checks if mario is near a ladder
{
	int distance;
	if (dir == GameConfig::UP) //Dir is UP
	{
		for (int i = 0; i < size; i++)
		{
			for (int currfloor = lad[i].getPos().getY(); currfloor > lad[i].getPos().getY() - lad[i].getSteps(); currfloor -= GameConfig::FLOORDIFF)
			{

				if (player->getPos().getY() == currfloor && (abs(distance = (player->getPos().getX() - lad[i].getPos().getX())) <= 1))
				{
					if (distance == 1)
						player->setPos(player->getPos().getX() - 1, player->getPos().getY());
					if (distance == -1)
						player->setPos(player->getPos().getX() + 1, player->getPos().getY());
					*ladderindex = i;
					*climb = (currfloor - (lad[i].getPos().getY() - lad[i].getSteps()) + 1);
					return lad[i].getSteps() + 1;
				}

			}
		}
	}
	else //Down
	{

		for (int i = 0; i < size; i++)
		{
			for (int currfloor = lad[i].getPos().getY()-lad[i].getSteps(); currfloor < lad[i].getPos().getY(); currfloor += GameConfig::FLOORDIFF)
			{

				if (player->getPos().getY() == currfloor && (abs(distance = (player->getPos().getX() - lad[i].getPos().getX())) <= 1))
				{
					if (distance == 1)
						player->setPos(player->getPos().getX() - 1, player->getPos().getY());
					if (distance == -1)
						player->setPos(player->getPos().getX() + 1, player->getPos().getY());
					*ladderindex = i;
					*climb = (lad[i].getPos().getY() - currfloor)+1;
					return lad[i].getSteps() + 1;
				}

			}
		}
	}
	
	

	return 0;
}


int main()
{
	Level level(3);
	level.setBoardValue(0, 0, 1); // Plain brick
	level.setBoardValue(1, 1, 2); // Right slope
	level.setBoardValue(2, 2, 3); // Left slope

	// Set ladders
	level.setLadder(0, Ladder(Point(0, 0), 5));
	level.setLadder(1, Ladder(Point(1, 1), 4));
	level.setLadder(2, Ladder(Point(2, 2), 6));

	// Floor 0: Mostly plain bricks with small gaps
	for (int col = 0; col < GameConfig::WIDTH - 2; ++col) {
		if (col % 10 < 8) {
			level.setBoardValue(0, col, 1); // Plain bricks
		}
		else {
			level.setBoardValue(0, col, 0); // Small gaps
		}
	}

	// Floor 1: Right slope at the start, some gaps, and plain bricks
	level.setBoardValue(1, 0, 2); // Right slope
	for (int col = 1; col < GameConfig::WIDTH - 2; ++col) {
		if (col % 12 < 9) {
			level.setBoardValue(1, col, 1); // Plain bricks
		}
		else {
			level.setBoardValue(1, col, 0); // Gaps
		}
	}

	// Floor 2: Left slope near the end, fewer gaps
	for (int col = 0; col < GameConfig::WIDTH - 3; ++col) {
		if (col % 15 == 13) {
			level.setBoardValue(2, col, 3); // Left slope
		}
		else if (col % 12 < 8) {
			level.setBoardValue(2, col, 1); // Plain bricks
		}
		else {
			level.setBoardValue(2, col, 0); // Gaps
		}
	}

	// Floor 3: Mix of slopes and gaps
	for (int col = 0; col < GameConfig::WIDTH - 2; ++col) {
		if (col % 20 == 5) {
			level.setBoardValue(3, col, 2); // Right slope
		}
		else if (col % 18 == 9) {
			level.setBoardValue(3, col, 3); // Left slope
		}
		else if (col % 7 < 6) {
			level.setBoardValue(3, col, 1); // Plain bricks
		}
		else {
			level.setBoardValue(3, col, 0); // Gaps
		}
	}

	// Floor 4-7: Alternating plain bricks and slopes
	for (int floor = 4; floor < GameConfig::NUMFLOORS; ++floor) {
		for (int col = 0; col < GameConfig::WIDTH - 2; ++col) {
			if ((floor + col) % 16 == 0) {
				level.setBoardValue(floor, col, 2); // Right slope
			}
			else if ((floor + col) % 18 == 0) {
				level.setBoardValue(floor, col, 3); // Left slope
			}
			else if (col % 9 < 8) {
				level.setBoardValue(floor, col, 1); // Plain bricks
			}
			else {
				level.setBoardValue(floor, col, 0); // Gaps
			}
		}
	}

	level.printBoard();






	GameObject mario(Point(), '@');
	GameObject pauline(Point(GameConfig::MIN_X+2,GameConfig::FLOORS::FLOOR8), '$');
	pauline.draw();
	drawBorders();
	//drawFloors();
	Ladder l(Point(GameConfig::MIN_X + 10, GameConfig::FLOOR1 - 1),4);
	Ladder l1(Point(GameConfig::MIN_X + 30, GameConfig::FLOOR1 - 1), 2);
	Ladder arrladders[2] = { l,l1 };

	for (auto lad : arrladders)
		lad.draw();

	//Jump var
	int wPressed = 0;

	char keyPressed = 0;

	//Ladder vars
	int climb=0;
	int ladderSteps = 0;
	GameConfig::ARROWKEYS laddermotionprev;
	int indexofCurrLadder = -1;

	do {
		
		if (!wPressed) //Not Jump Mode
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
					else //Climb Mode
					{
						if (mario.getDir() == GameConfig::ARROWKEYS::STAY && LeaveLadder(mario.getPos().getY(), arrladders[indexofCurrLadder])) //Able to leave the ladder
						{
							climb = 0;
							mario.setDir(GameConfig::ARROWKEYS::LEFT);
						}
							

					}
					break;

				case 'd':
				case 'D':
					if(climb==0)
					mario.setDir(GameConfig::ARROWKEYS::RIGHT);
					else //Climb Mode
					{
						if (mario.getDir() == GameConfig::ARROWKEYS::STAY && LeaveLadder(mario.getPos().getY(), arrladders[indexofCurrLadder])) //Able to leave the ladder
						{
							climb = 0;
							mario.setDir(GameConfig::ARROWKEYS::RIGHT);
						}

					}

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
						if (currstate == GameConfig::ARROWKEYS::DOWN||(currstate == GameConfig::ARROWKEYS::STAY&& laddermotionprev == GameConfig::DOWN)) //Change Direction
						{
							
							climb = (ladderSteps - climb)+1;
							
						}
						laddermotionprev = GameConfig::ARROWKEYS::UP;
						mario.setDir(GameConfig::ARROWKEYS::UP);
						
					}
					else //Not climb mode
					{
						if (currstate ==GameConfig::STAY &&(ladderSteps=nearLadder(&mario,arrladders,2,GameConfig::UP,&indexofCurrLadder,&climb))!=0)//Mario is near a ladder
						{
							laddermotionprev = GameConfig::ARROWKEYS::UP;
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
						if (mario.getDir() == GameConfig::STAY && (ladderSteps = nearLadder(&mario, arrladders, 2, GameConfig::DOWN,&indexofCurrLadder,&climb)) != 0)
						{
							laddermotionprev = GameConfig::ARROWKEYS::DOWN;
							mario.setDir(GameConfig::ARROWKEYS::DOWN);
						}
					}
					else //On climb mode
					{
						if (mario.getDir() == GameConfig::ARROWKEYS::UP ||( mario.getDir() == GameConfig::ARROWKEYS::STAY&& laddermotionprev == GameConfig::UP))
						{
							
							climb = (ladderSteps - climb)+1;
							
						}
						laddermotionprev = GameConfig::ARROWKEYS::DOWN;
						mario.setDir(GameConfig::ARROWKEYS::DOWN);
							
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
