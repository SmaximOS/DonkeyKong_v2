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
		cout << 'Q'; // Top border
		gotoxy(x, GameConfig::MIN_Y + GameConfig::HEIGHT);
		cout << 'Q'; // Bottom border
	}

	// Left and right borders
	for (int y = GameConfig::MIN_Y; y < GameConfig::MIN_Y + GameConfig::HEIGHT; ++y) {
		gotoxy(GameConfig::MIN_X, y);
		cout << 'Q'; // Left border
		gotoxy(GameConfig::MIN_X + GameConfig::WIDTH, y);
		cout << 'Q'; // Right border
	}
	gotoxy(GameConfig::MIN_X + GameConfig::WIDTH, GameConfig::MIN_Y + GameConfig::HEIGHT);
	cout << 'Q';
}

void printLives(int lives)
{
	gotoxy(GameConfig::MIN_X + GameConfig::WIDTH + 3, GameConfig::MIN_Y);
	cout << "Lives:" << lives;
}
int getFloor(int ycoor)
{
	return (((GameConfig::FLOOR1 - 1) - ycoor) / GameConfig::FLOORDIFF);
}
char getSlope(Point currpos, char board[][GameConfig::WIDTH - 2])
{

	int row = getFloor(currpos.getY());
	int col = currpos.getX() - (GameConfig::MIN_X+1);
	int right, left;
    right = left = col;
	bool stopSearch = false;
	while (stopSearch==false&&(board[row][right] <= 1 && board[row][left] <= 1))
	{
		stopSearch = true;
		if (right < GameConfig::WIDTH - 3&& board[row][right]!=0)
		{
			stopSearch = false;
			right++;
		}
		if (left > 0 && board[row][left] != 0)
		{
			stopSearch = false;
			left--;
		}
			
	}

	if (stopSearch)
		return 1;
	else if (board[row][right] > 1)
		return board[row][right];
	else
		return board[row][left];

}
bool LeaveLadder(Point currPos, Ladder lad,GameConfig::ARROWKEYS dir, char board[][GameConfig::WIDTH - 2])// Checks if there is an option to leave the ladder int the middle
{
	int ypos = currPos.getY();
	int xpos = currPos.getX();
	for (int middlefloor = lad.getPos().getY() - GameConfig::FLOORDIFF; middlefloor > lad.getPos().getY() - lad.getSteps() ; middlefloor -= GameConfig::FLOORDIFF)
	{
		if (ypos == middlefloor)
		{
			int indexFloor = getFloor(ypos);
			if (dir == GameConfig::LEFT && board[indexFloor][xpos - (GameConfig::MIN_X + 1) - 1] != 0)
				return true;
			else if (dir == GameConfig::RIGHT && board[indexFloor][xpos - (GameConfig::MIN_X + 1) + 1] != 0)
				return true;
			return false;
		}
			
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
	Level level =Level();
	level.initializeBoard();
	level.printBoard();
	drawBorders();

	bool finished = false;
	GameObject mario(Point(), '@');
	GameObject pauline(Point(GameConfig::MIN_X+2,GameConfig::FLOORS::FLOOR8-1), '$');
	pauline.draw();
	
	//Jump var
	int wPressed = 0;

	//Falling down var
	int descent = 0;

	//Lives
	int lives = 3;

	char keyPressed = 0;

	//Ladder vars
	int climb=0;
	int ladderSteps = 0;
	GameConfig::ARROWKEYS laddermotionprev;
	int indexofCurrLadder = -1;

	do {
		
		if (!wPressed) //Not Jump Mode
		{
			
			if (_kbhit()&&descent==0) //Confirms the player is not falling down while hitting the button
			{
				keyPressed = _getch();

				switch (keyPressed)
				{
				case 'a':
				case  'A':
					if (climb == 0)
					{
						mario.setDir(GameConfig::ARROWKEYS::LEFT);
					}
					
					else //Climb Mode
					{
						if (mario.getDir() == GameConfig::ARROWKEYS::STAY && LeaveLadder(mario.getPos(), level.getLadder(indexofCurrLadder),GameConfig::LEFT,level.getBoardPointer())) //Able to leave the ladder
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
						if (mario.getDir() == GameConfig::ARROWKEYS::STAY && LeaveLadder(mario.getPos(), level.getLadder(indexofCurrLadder), GameConfig::RIGHT, level.getBoardPointer())) //Able to leave the ladder
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
						if (currstate ==GameConfig::STAY &&(ladderSteps=nearLadder(&mario,level.getLadders(), level.getNumLadders(), GameConfig::UP, &indexofCurrLadder, &climb)) != 0)//Mario is near a ladder
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
						if (mario.getDir() == GameConfig::STAY && (ladderSteps = nearLadder(&mario, level.getLadders(), level.getNumLadders(), GameConfig::DOWN, &indexofCurrLadder, &climb)) != 0)
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
			else if (descent > 0) //Falling Down
			{
				if (descent % 4 == 0) // checks if the fall shoud stop
				{
					int currFloor = getFloor(mario.getPos().getY());
					if (level.getBoardValue(currFloor, mario.getPos().getX()-(GameConfig::MIN_X+1)) != 0)
					{
						if (descent >= GameConfig::FLOORDIFF * 3)
							lives--;
						switch (mario.getDir())
						{
						case GameConfig::DOWN:
							mario.setDir(GameConfig::STAY);
							break;
						case GameConfig::DOWNANDRIGHT:
							mario.setDir(GameConfig::RIGHT);
							break;
						case GameConfig::DOWNANDLEFT:
							mario.setDir(GameConfig::LEFT);
							break;

						default:
							break;
						}
						descent = 0;
					}
				}
				else if (descent % 4 == 1&&mario.getDir()!=GameConfig::DOWN) //IN Case mario faces a brick while falling diagonally
				{
					int floortoCheck = getFloor(mario.getPos().getY()) + 1;
					char element = level.getBoardValue(floortoCheck, (mario.getPos().getX()) - (GameConfig::MIN_X + 1));
					gotoxy(mario.getPos().getX(), mario.getPos().getY());
					if (element != 0)
					{
						switch (element)
						{
						case 1:
							cout << '=';
							break;
						case 2:
							cout << '>';
							break;
						case 3:
							cout << '<';
							break;
						}
					}
				}
				if(descent!=0)
				descent++;
			}
			else //Check if mario reached an edge on regular mode
			{
				int currFloor = getFloor(mario.getPos().getY());
				if (level.getBoardValue(currFloor, mario.getPos().getX() - (GameConfig::MIN_X + 1)) == 0)
				{
					if (mario.getDir() == GameConfig::RIGHT)
					{
						mario.setDir(GameConfig::DOWNANDRIGHT);
						descent++;
					}
					else
					{
						mario.setDir(GameConfig::DOWNANDLEFT);
						descent++;
					}
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
					int currFloor = getFloor(mario.getPos().getY());
					GameConfig::ARROWKEYS currstate = mario.getDir();
					if (currstate == GameConfig::ARROWKEYS::DOWNANDLEFT)
					{
						if (level.getBoardValue(currFloor, mario.getPos().getX() - (GameConfig::MIN_X + 1)) != 0)
							mario.setDir(GameConfig::ARROWKEYS::LEFT);
						else
							descent++;
					}
						
					else if (currstate == GameConfig::DOWNANDRIGHT)
					{
						if (level.getBoardValue(currFloor, mario.getPos().getX() - (GameConfig::MIN_X + 1)) != 0)
							mario.setDir(GameConfig::ARROWKEYS::RIGHT);
						else
							descent++;
					}
						
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
			else if (mario.getDir() == GameConfig::DOWNANDLEFT)
				mario.setDir(GameConfig::DOWN);
			else if (keyPressed == 'd' || keyPressed == 'D')
				mario.setDir(GameConfig::RIGHT);
			else if(descent==0)
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
					else if(descent==0)
						mario.setDir(GameConfig::ARROWKEYS::STAY);
				}
			}
			else if (mario.getDir() == GameConfig::DOWNANDRIGHT)
				mario.setDir(GameConfig::DOWN);
			else if (keyPressed == 'a' || keyPressed == 'A')
				mario.setDir(GameConfig::LEFT);
			else if(descent==0)
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
		
		
		level.printLadders();
		printLives(lives);

		mario.move();
		mario.draw();
		Sleep(200);
		gotoxy(mario.getPos().getX(), mario.getPos().getY());
		cout << " ";
		
		if (mario.getPos() == pauline.getPos())
		{
			finished = true;
			break;
		}
	} while (true);
	
	gotoxy(0,GameConfig::HEIGHT + GameConfig::MIN_Y+1);
	if (finished)
		cout << "Game Won" << endl;
	return 0;
}
