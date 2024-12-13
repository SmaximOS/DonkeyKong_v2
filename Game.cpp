#include "GameObject.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include "general.h"
#include "Ladder.h"
#include "Level.h"
#include <vector>

using namespace std;

void startMenu() {
	system("cls");
	cout << "======================== Donkey Kong ============================== \n";
	cout << "\n(1)Start a new game\n";
	cout << "(8)instructions and game controls\n";
	cout << "(9)Leave game\n";
	cout << "=================================================================== \n";
	cout << "Please select an option";
}

void showInstructions() {
	system("cls");  
	cout << "============================ Instructions ==========================\n";
	cout << "\nIn this game,, you play as Mario.\n";
	cout << "Mario is given 3 chances (lives) to reach Pauline, which will be displayed in the upper-right corner of the screen\n";
	cout << "Mario loses a life and the game restarts if he faces a barrel, falls to the abyss, falls for 3 or more floors, or find himself near a barrels explosion (2 Characters difference).\n";
	cout << "====================================================================\n";
	cout << "press any key to see the game controls...";
	_getch();
	system("cls");
	cout << "============================ game controls ==========================\n";
	cout << "Use the following keys to play the game:\n";
	cout << "A / D - Move Left / Right\n";
	cout << "W - Jump\n";
	cout << "S - Stay\n";
	cout << "X - Tumble Down a Ladder\n";
	cout << "Space - Pause the Game\n";
	cout << "====================================================================\n";
	cout << "Press any key to return to the main menu";
	_getch();  
}

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
	if (ycoor >= GameConfig::FLOOR1)
		return -1;
	return (((GameConfig::FLOOR1 - 1) - ycoor) / GameConfig::FLOORDIFF);
}
char getSlope(Point currpos, char board[][GameConfig::WIDTH - 2])
{

	int row = getFloor(currpos.getY());
	int col = currpos.getX() - (GameConfig::MIN_X + 1);
	int right, left;
	right = left = col;
	bool stopSearch = false;
	while (stopSearch == false && (board[row][right] <= 1 && board[row][left] <= 1))
	{
		stopSearch = true;
		if (right < GameConfig::WIDTH - 3 && board[row][right] != 0)
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
bool LeaveLadder(Point currPos, Ladder lad, GameConfig::ARROWKEYS dir, char board[][GameConfig::WIDTH - 2])// Checks if there is an option to leave the ladder int the middle
{
	int ypos = currPos.getY();
	int xpos = currPos.getX();
	for (int middlefloor = lad.getPos().getY() - GameConfig::FLOORDIFF; middlefloor > lad.getPos().getY() - lad.getSteps(); middlefloor -= GameConfig::FLOORDIFF)
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


int nearLadder(GameObject* player, Ladder lad[], int size, GameConfig::ARROWKEYS dir, int* ladderindex, int* climb) //Checks if mario is near a ladder
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
			for (int currfloor = lad[i].getPos().getY() - lad[i].getSteps(); currfloor < lad[i].getPos().getY(); currfloor += GameConfig::FLOORDIFF)
			{

				if (player->getPos().getY() == currfloor && (abs(distance = (player->getPos().getX() - lad[i].getPos().getX())) <= 1))
				{
					if (distance == 1)
						player->setPos(player->getPos().getX() - 1, player->getPos().getY());
					if (distance == -1)
						player->setPos(player->getPos().getX() + 1, player->getPos().getY());
					*ladderindex = i;
					*climb = (lad[i].getPos().getY() - currfloor) + 1;
					return lad[i].getSteps() + 1;
				}

			}
		}
	}



	return 0;
}
bool barrelsCheckHits(vector<Barrel>* barrels, GameObject playerPosition) //Checks if two barrels hit and if mario is near the hit
{
	bool currIndexdeleted;
	for (int i = 0;i < (barrels->size());i++)
	{
		currIndexdeleted = false;
		for (int j = i+1;j < (barrels->size());j++)
		{
			if (barrels->at(j).getPos().calculateDistance(barrels->at(i).getPos())<=1)//Explosion
			{
				if ((playerPosition.getPos().calculateDistance(barrels->at(j).getPos()) <= 2)|| playerPosition.getPos().calculateDistance(barrels->at(i).getPos())<=2)
					return true; //Mario is near the hit
				currIndexdeleted = true;
				barrels->erase(barrels->begin() + j);
				j--;
			}
		}
		if (currIndexdeleted)
		{
			barrels->erase(barrels->begin() + i);
			i--;
		}
		
	}
	return false; //Mario wasnt near a barrels hit 
		
}
int barrelDistanceFloor(Barrel bar, int floor) //The distance between the barrel and the floor
{
	return ((GameConfig::MIN_Y + GameConfig::HEIGHT - 1)-bar.getPos().getY() - (GameConfig::FLOORDIFF * floor)-1);
}
void barrelsUpdateDirs(vector<Barrel>* barrels, char board[][GameConfig::WIDTH - 2])
{
	for (auto& barrel : *barrels)
	{
		GameConfig::ARROWKEYS currDir = barrel.getDir();
		int floor = getFloor(barrel.getPos().getY());
		int barreldistfloor = barrelDistanceFloor(barrel, floor);
		switch (currDir)
		{

		 case GameConfig::ARROWKEYS::RIGHT:
		 case GameConfig::ARROWKEYS::LEFT:
		 {
			 if (board[floor][barrel.getPos().getX() - (GameConfig::MIN_X + 1)] == 0) //Barrel falls
			 {
				 if (currDir == GameConfig::ARROWKEYS::RIGHT)
					 barrel.setDir(GameConfig::DOWNANDRIGHT);
				 else //Direction is left
					 barrel.setDir(GameConfig::DOWNANDLEFT);
			 }
			 break;
		 }
		
		 case GameConfig::ARROWKEYS::DOWNANDRIGHT:
		 case GameConfig::ARROWKEYS::DOWNANDLEFT:
		 { 

			if (barreldistfloor==0 && board[floor][barrel.getPos().getX()-(GameConfig::MIN_X+1)] != 0) //Barrel fall should stop
			{
				int slope = getSlope(barrel.getPos(), board);

				if (slope == 1) //Plain
				{
					if (currDir == GameConfig::ARROWKEYS::DOWNANDRIGHT)
						barrel.setDir(GameConfig::ARROWKEYS::RIGHT);
					else
						barrel.setDir(GameConfig::ARROWKEYS::LEFT);
				}
				else if (slope == 2)
					barrel.setDir(GameConfig::ARROWKEYS::RIGHT);
				else
					barrel.setDir(GameConfig::ARROWKEYS::LEFT);
			}
			else if (barreldistfloor == GameConfig::FLOORDIFF - 1 && board[floor+1][barrel.getPos().getX() - (GameConfig::MIN_X + 1)] != 0) //Barrel hits a brick while falling
			{

				char element = board[floor+1][barrel.getPos().getX()-(GameConfig::MIN_X+1)];
				gotoxy(barrel.getPos().getX(), barrel.getPos().getY());
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
			break;
		  }
		}
	}

}
bool marioHitsBarrel(vector<Barrel> barrels,GameObject mario)
{
	for (auto& barrel : barrels)
	{
		if (mario.getPos() == barrel.getPos())
			return true;
	}
	return false;
}
bool outOfBounds(Point pos)
{
	return pos.getX() < GameConfig::MIN_X + 2 || pos.getX() > GameConfig::MIN_X + GameConfig::WIDTH - 2 || pos.getY() > GameConfig::MIN_Y + GameConfig::HEIGHT - 1;
}
void printBarrelTraces(vector<Barrel> barrels)
{
	for (auto& barrel : barrels)
	{
		gotoxy(barrel.getPos().getX(), barrel.getPos().getY());
		cout << ' ';
	}
}
void pauseGame(GameObject mario,vector<Barrel> barrels)
{
	gotoxy(0, GameConfig::HEIGHT + GameConfig::MIN_Y + 1);
	cout << "Game Paused";
	char keyPressed = 0;
	mario.draw();
	for (auto& bar : barrels)
		bar.draw();
	while (keyPressed != GameConfig::SPACE)
	{
		if (_kbhit())
			keyPressed = _getch();
	}

	//Print ' ' after mario
	gotoxy(mario.getPos().getX(), mario.getPos().getY());
	cout << " ";

	//print ' ' after the barrels
	printBarrelTraces(barrels);

	//Delete the pause game caption
	gotoxy(0, GameConfig::HEIGHT + GameConfig::MIN_Y + 1);
	cout << "            ";
}
void restart(GameObject* mario,Point marioStartPos,vector<Barrel>* barrels,int* timetonextbarrel,int* climb,int* jumpsecs)
{
	//Mario initial position
	mario->setPos(marioStartPos.getX(), marioStartPos.getY());
	mario->setDir(GameConfig::STAY);

	//Delete all barrels
	barrels->clear();

	//Reset other variables
	*timetonextbarrel = *climb = *jumpsecs = 0;
}
int main()
{
	bool gameRunning = false;
	int menuOption = 0;

	while (!gameRunning) {
		startMenu();  
		menuOption = _getch() - '0';  

		switch (menuOption) {
		case 1:
			gameRunning = true;  
			break;
		case 8:
			// Show instructions
			showInstructions();
			break;
		case 9:
			// Exit the game
			cout << "\nExiting game...\n";
			return 0;  // Exit the program
		default:
			// Invalid option
			cout << "Invalid choice, please try again.\n";
			break;
		}
	}
	system("cls");
	ShowConsoleCursor(false);
	Level level=Level();
	level.initializeBoard1();
	level.printBoard();
	drawBorders();
	bool finished = false;

	GameObject mario(level.getstartPosMario(), '@');
	GameObject pauline(level.getstartPosPauline(), '$');
	GameObject donkeyKong(level.getstartPosDonkeyKong(), '&');

	donkeyKong.draw();
	pauline.draw();

	//Jump var
	int wPressed = 0;

	//Falling down var
	int descent = 0;

	//Lives
	int lives = 3;

	char keyPressed = 0;

	//Ladder vars
	int climb = 0;
	int ladderSteps = 0;
	GameConfig::ARROWKEYS laddermotionprev;
	int indexofCurrLadder = -1;

	//GameSeconds
	int timePlayed = 0;

	//Barrels
	vector<Barrel> barrels;
	LevelSettings currSettings = level.getLevelSettings();
	int timetonextbarrel = 0;
	int currbarrelindex = 0;


	do {

		if (!wPressed) //Not Jump Mode
		{

			if (_kbhit() && descent == 0) //Confirms the player is not falling down while hitting the button
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
						if (mario.getDir() == GameConfig::ARROWKEYS::STAY && LeaveLadder(mario.getPos(), level.getLadder(indexofCurrLadder), GameConfig::LEFT, level.getBoardPointer())) //Able to leave the ladder
						{
							climb = 0;
							mario.setDir(GameConfig::ARROWKEYS::LEFT);
						}


					}
					break;

				case 'd':
				case 'D':
					if (climb == 0)
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
						if (currstate == GameConfig::ARROWKEYS::DOWN || (currstate == GameConfig::ARROWKEYS::STAY && laddermotionprev == GameConfig::DOWN)) //Change Direction
						{

							climb = (ladderSteps - climb) + 1;

						}
						laddermotionprev = GameConfig::ARROWKEYS::UP;
						mario.setDir(GameConfig::ARROWKEYS::UP);

					}
					else //Not climb mode
					{
						if (currstate == GameConfig::STAY && (ladderSteps = nearLadder(&mario, level.getLadders(), level.getNumLadders(), GameConfig::UP, &indexofCurrLadder, &climb)) != 0)//Mario is near a ladder
						{
							laddermotionprev = GameConfig::ARROWKEYS::UP;
							mario.setDir(GameConfig::ARROWKEYS::UP);
						}
						else if(descent==0) //Regular Jump, Not allowed while falling
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
						if (mario.getDir() == GameConfig::ARROWKEYS::UP || (mario.getDir() == GameConfig::ARROWKEYS::STAY && laddermotionprev == GameConfig::UP))
						{

							climb = (ladderSteps - climb) + 1;

						}
						laddermotionprev = GameConfig::ARROWKEYS::DOWN;
						mario.setDir(GameConfig::ARROWKEYS::DOWN);

					}
					break;
				}
				case GameConfig::SPACE:
					pauseGame(mario,barrels);
				default:
					break;
				}

			}
			if (climb > 0) //Climb Mode
			{
				if (mario.getDir() != GameConfig::ARROWKEYS::STAY)
					climb--;
				if (climb == 0)
					mario.setDir(GameConfig::ARROWKEYS::STAY);
			}
			else if (descent > 0) //Falling Down
			{
				if (descent % 4 == 0) // checks if the fall shoud stop
				{
					int currFloor = getFloor(mario.getPos().getY());
					if (level.getBoardValue(currFloor, mario.getPos().getX() - (GameConfig::MIN_X + 1)) != 0)
					{
						if (descent >= GameConfig::FLOORDIFF * 3) //Mario fell 3 floors or more
						{
							lives--;
							restart(&mario, level.getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed);
						}
							
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
				else if (descent % 4 == 1 && mario.getDir() != GameConfig::DOWN) //IN Case mario faces a brick while falling diagonally
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
				if (descent != 0)
					descent++;
			}
			else if(wPressed==0)//Check if mario reached an edge on regular mode,Relevant for non-jumping  mode
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
			if (wPressed - 1 == GameConfig::JUMPSECS / 2)
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
			else if (descent == 0)
				mario.setDir(GameConfig::ARROWKEYS::STAY);

		}
		if (mario.getPos().getX() > GameConfig::MIN_X + GameConfig::WIDTH - 2)// Reached Right Bound
		{
			if (wPressed > 0)// Jump Mode
			{
				if ((mario.getDir() != GameConfig::ARROWKEYS::UP) && (mario.getDir() != GameConfig::ARROWKEYS::DOWN))// Diagonal Jump Mode
				{
					if (wPressed - 1 >= GameConfig::JUMPSECS / 2)
						wPressed = GameConfig::JUMPSECS - wPressed;
					if (wPressed != 0)
						mario.setDir(GameConfig::ARROWKEYS::DOWN);
					else if (descent == 0)
						mario.setDir(GameConfig::ARROWKEYS::STAY);
				}
			}
			else if (mario.getDir() == GameConfig::DOWNANDRIGHT)
				mario.setDir(GameConfig::DOWN);
			else if (keyPressed == 'a' || keyPressed == 'A')
				mario.setDir(GameConfig::LEFT);
			else if (descent == 0)
				mario.setDir(GameConfig::ARROWKEYS::STAY);
		}
		if (mario.getPos().getY() >= GameConfig::MIN_Y + GameConfig::HEIGHT - 1) //Mario Fell Down
		{
			lives--;
			restart(&mario, level.getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed);
		}

		//Barrels
		if (timetonextbarrel == currSettings.intervalsBetweenBarrels[currbarrelindex]) //Time to add next barrel
		{
			barrels.push_back(Barrel(donkeyKong.getPos(), currSettings.dirs[currbarrelindex]));
			currbarrelindex++;
			if (currbarrelindex == currSettings.size)
				currbarrelindex = 0;
			timetonextbarrel = 0;
		}
		else
			timetonextbarrel++;

		barrelsUpdateDirs(&barrels, level.getBoardPointer());//Set the exact direction for each barrel

		for (auto& barel : barrels) //Move the barrels
			barel.move();
		
		if (barrelsCheckHits(&barrels, mario)) //delete barrels that share same position (explosion)
		{
			//Mario is near an explosion
			lives--;
			restart(&mario, level.getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed);
		}
			
		
		for (int i = 0;i < barrels.size();i++) //erasing all the barrels that are out of bounds
		{
			if (outOfBounds(barrels[i].getPos()))
				barrels.erase(barrels.begin() + i);
		}
		mario.move();
		if (marioHitsBarrel(barrels, mario))
		{
			//mario hit a barrel
			lives--;
			restart(&mario, level.getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed);
		}
		level.printLadders();
		printLives(lives);
		mario.draw();
		for (auto& barel : barrels) //Draw the barrels
			barel.draw();
		Sleep(170);

		//Print ' ' after mario
		gotoxy(mario.getPos().getX(), mario.getPos().getY());
		cout << " ";
		//Print ' ' after the barrels
		printBarrelTraces(barrels);
		

		if (mario.getPos() == pauline.getPos())
		{
			finished = true;
			break;
		}
	} while (lives>0);

	
	if (finished)
	{
		gotoxy(0, GameConfig::HEIGHT + GameConfig::MIN_Y + 1);
		cout << "Game Won" << endl;
	}
	else
	{
		gotoxy(0, GameConfig::HEIGHT + GameConfig::MIN_Y + 1);
		cout << "Failure" << endl;
	}
		
	return 0;
}
