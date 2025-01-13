#include "Game.h"


void Game::startMenu() {
	system("cls");
	std::cout << "======================== Donkey Kong ============================== \n";
	std::cout << "\n(1)Start a new game\n";
	std::cout << "(8)instructions and game controls\n";
	std::cout << "(9)Leave game\n";
	std::cout << "=================================================================== \n";
	std::cout << "Please select an option";
}

void Game:: showInstructions() {
	system("cls");
	std::cout << "============================ Instructions ==========================\n";
	std::cout << "\nIn this game,, you play as Mario.\n";
	std::cout << "Mario is given 3 chances (lives) to reach Pauline, which will be displayed in the upper-right corner of the screen\n";
	std::cout << "Mario loses a life and the game restarts if he faces a barrel, falls to the abyss, falls for 3 or more floors, or find himself near a barrels explosion (2 Characters difference).\n";
	std::cout << "====================================================================\n";
	std::cout << "press any key to see the game controls...";
	_getch();
	system("cls");
	std::cout << "============================ game controls ==========================\n";
	std::cout << "Use the following keys to play the game:\n";
	std::cout << "A / D - Move Left / Right\n";
	std::cout << "W - Jump\n";
	std::cout << "S - Stay\n";
	std::cout << "X - Tumble Down a Ladder\n";
	std::cout << "Space - Pause the Game\n";
	std::cout << "====================================================================\n";
	std::cout << "Press any key to return to the main menu";
	_getch();
}

int Game:: lvlSelect() {
	system("cls");
	std::cout << "========================= Level selection ==========================\n";
	std::cout << "Select the level you wish to play on: \n";
	std::cout << "(1)\n(2)\n";
	std::cout << "====================================================================\n";
	char keyPressed = _getch();
	switch (keyPressed) {
	case '1':
		return 1;

	case '2':
		return 2;
	}
}

void Game:: drawBorders()
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


void Game:: printLives(int lives,const Point& legend)
{
	gotoxy(legend.getX(),legend.getY());
	std::cout << "Lives:" << lives;
}
int Game:: showTime(const Point& legend, bool reset = false)
{
	static int secs;
	if (reset)
	{
		int secscopy = secs;
		secs = 0;
		return secscopy;
	}
		
	else
	{
	int minutes = secs / 60;
	int seconds = secs % 60;

	// Display time in "MM:SS" format (Given by ChatGpt)
	gotoxy(legend.getX(), legend.getY()+1);
	std::cout << "Time: ";
	gotoxy(legend.getX(), legend.getY() + 2);
	std::cout << setw(2) << setfill('0') << minutes << ":"
		<< setw(2) << setfill('0') << seconds
		<< endl;
	return secs++;
    }
}
int Game:: getFloor(int ycoor) //an index from 0 to 7
{
	int min_y = currLevel->getFloorYcoor(0);
	if (ycoor >=min_y)
		return -1;
	return (((min_y - 1) - ycoor) / GameConfig::FLOORDIFF);
}
char Game:: getSlope(Point currpos, char board[][GameConfig::WIDTH - 2])
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
bool Game:: LeaveLadder(const Point& currPos,const Ladder& lad, GameConfig::ARROWKEYS dir, char board[][GameConfig::WIDTH - 2])// Checks if there is an option to leave the ladder int the middle
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


int Game:: nearLadder(Player* player, Ladder lad[], int size, GameConfig::ARROWKEYS dir, int* ladderindex, int* climb) //Checks if mario is near a ladder
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
						player->setPos({ player->getPos().getX() - 1, player->getPos().getY() });
					if (distance == -1)
						player->setPos({ player->getPos().getX() + 1, player->getPos().getY() });
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
						player->setPos({ player->getPos().getX() - 1, player->getPos().getY() });
					if (distance == -1)
						player->setPos({player->getPos().getX() + 1, player->getPos().getY()});
					*ladderindex = i;
					*climb = (lad[i].getPos().getY() - currfloor) + 1;
					return lad[i].getSteps() + 1;
				}

			}
		}
	}



	return 0;
}
bool Game:: barrelsCheckHits(vector<Barrel>* barrels,const Player& playerPosition) //Checks if two barrels hit and if mario is near the hit
{
	bool currIndexdeleted;
	for (int i = 0;i < (barrels->size());i++)
	{
		currIndexdeleted = false;
		for (int j = i + 1;j < (barrels->size());j++)
		{
			if (barrels->at(j).getPos().calculateDistance(barrels->at(i).getPos()) <= 1)//Explosion
			{
				if ((playerPosition.getPos().calculateDistance(barrels->at(j).getPos()) <= 2) || playerPosition.getPos().calculateDistance(barrels->at(i).getPos()) <= 2)
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
int Game:: barrelDistanceFloor(const Barrel& bar, int floor) //The distance between the barrel and the floor
{
	return ((GameConfig::MIN_Y + GameConfig::HEIGHT - 1) - bar.getPos().getY() - (GameConfig::FLOORDIFF * floor) - 1);
}
bool Game:: barrelsUpdateDirs(vector<Barrel>* barrels, char board[][GameConfig::WIDTH - 2],Player* mario)
{
	for (int i=0;i<barrels->size();i++)
	{
		GameConfig::ARROWKEYS currDir = barrels->at(i).getDir();
		int floor = getFloor(barrels->at(i).getPos().getY());
		int barreldistfloor = barrelDistanceFloor(barrels->at(i), floor);
		switch (currDir)
		{

		case GameConfig::ARROWKEYS::RIGHT:
		case GameConfig::ARROWKEYS::LEFT:
		{
			if (board[floor][barrels->at(i).getPos().getX() - (GameConfig::MIN_X + 1)] == 0) //Barrel falls
			{
				if (currDir == GameConfig::ARROWKEYS::RIGHT)
					barrels->at(i).setDir(GameConfig::DOWNANDRIGHT);
				else //Direction is left
					barrels->at(i).setDir(GameConfig::DOWNANDLEFT);
			}
			break;
		}

		case GameConfig::ARROWKEYS::DOWNANDRIGHT:
		case GameConfig::ARROWKEYS::DOWNANDLEFT:
		{

			if (barreldistfloor == 0 && board[floor][barrels->at(i).getPos().getX() - (GameConfig::MIN_X + 1)] != 0) //Barrel fall should stop
			{
				if (barrels->at(i).getFallSecs() >= 4 * (GameConfig::FLOORDIFF)) //Barrel fell to many floors and should explode
				{
					if (mario->getPos().calculateDistance(barrels->at(i).getPos()) <= 2) //Mario is near the explosion
						return false;

					barrels->erase(barrels->begin() + i);
					i--;
					continue;
				}
				else
				{
					barrels->at(i).setFallSecs(0); //Reset the time of falling
					int slope = getSlope(barrels->at(i).getPos(), board);
					if (slope == 1) //Plain
					{
						if (currDir == GameConfig::ARROWKEYS::DOWNANDRIGHT)
							barrels->at(i).setDir(GameConfig::ARROWKEYS::RIGHT);
						else
							barrels->at(i).setDir(GameConfig::ARROWKEYS::LEFT);
					}
					else if (slope == 2)
						barrels->at(i).setDir(GameConfig::ARROWKEYS::RIGHT);
					else
						barrels->at(i).setDir(GameConfig::ARROWKEYS::LEFT);
				}
			}
			else if (barreldistfloor == GameConfig::FLOORDIFF - 1 && board[floor + 1][barrels->at(i).getPos().getX() - (GameConfig::MIN_X + 1)] != 0) //Barrel hits a brick while falling
			{

				char element = board[floor + 1][barrels->at(i).getPos().getX() - (GameConfig::MIN_X + 1)];
				gotoxy(barrels->at(i).getPos().getX(), barrels->at(i).getPos().getY());
				if (element != 0)
				{
					switch (element)
					{
					case 1:
						std::cout << '=';
						break;
					case 2:
						std::cout << '>';
						break;
					case 3:
						std::cout << '<';
						break;
					}
				}
			}
			break;
		}
		}
	  
	}

	return true;
}
bool Game:: marioHitsBarrel(vector<Barrel>& barrels,const Player& mario)
{
	for (auto& barrel : barrels)
	{
		if (mario.getPos() == barrel.getPos())
			return true;
	}
	return false;
}
bool Game::marioHitsGhost(vector<Ghost>& ghosts,const Player& mario)
{
	for (auto& ghost : ghosts)
	{
		if (mario.getPos() == ghost.getPos())
			return true;
	}
	return false;
}
void Game::ghostsChangeDir(vector<Ghost>& ghosts)
{
	vector<Ghost*> floordiv[8]; 

	for (auto& ghost : ghosts) //Dividing the existing ghosts to subvectors differs by the floors
	{
		int currfloor;
		currfloor = getFloor(ghost.getPos().getY());
		floordiv[currfloor].push_back(&ghost);
	}

	for (int i = 0; i < 8; i++)
	{
		int currsize;
		if ((currsize=floordiv[i].size()) > 1) //Multiple ghosts in the same floor
		{

			for (int j = 0; j < currsize; j++)
			{
				for (int k = j+1; k < currsize; k++)
				{
					if (abs(floordiv[i].at(j)->getPos().getX() - floordiv[i].at(k)->getPos().getX()) <= 2) //Change direction is needed
					{

						if (floordiv[i].at(j)->getDir() == GameConfig::LEFT)
						{
							floordiv[i].at(j)->setDir(GameConfig::RIGHT);
							floordiv[i].at(k)->setDir(GameConfig::LEFT);
						}
						else
						{
							floordiv[i].at(j)->setDir(GameConfig::LEFT);
							floordiv[i].at(k)->setDir(GameConfig::RIGHT);
						}
							
					}
				}
			}
		}


	}

}
bool Game::outOfBounds(const Point& pos)
{
	return pos.getX() < GameConfig::MIN_X + 2 || pos.getX() > GameConfig::MIN_X + GameConfig::WIDTH - 2 || pos.getY() > GameConfig::MIN_Y + GameConfig::HEIGHT - 1;
}
void Game::printBarrelTraces(vector<Barrel> barrels)
{
	for (auto& barrel : barrels)
	{
		gotoxy(barrel.getPos().getX(), barrel.getPos().getY());
		std::cout << ' ';
	}
}
void Game::printGhostsTraces(const vector<Ghost>& ghosts)
{
	for (auto& ghost : ghosts)
	{
		gotoxy(ghost.getPos().getX(), ghost.getPos().getY());
		std::cout << ' ';
	}
}


void Game::printMarioTrace(const Player& mario,const int& climb)
{
	//Print ' ' after mario
	gotoxy(mario.getPos().getX(), mario.getPos().getY());
	std::cout << " ";

	//Print ' ' after the hammer if mario owns it
	if (mario.getHammer() != GameConfig::ARROWKEYS::STAY && climb == 0 && (mario.getDir() == GameConfig::STAY || mario.getDir() == GameConfig::LEFT || mario.getDir() == GameConfig::RIGHT))
	{
		if (mario.getHammer() == GameConfig::ARROWKEYS::RIGHT)
		{
			gotoxy(mario.getPos().getX() + 1, mario.getPos().getY() - 1);
			std::cout << " ";
		}
		else
		{
			gotoxy(mario.getPos().getX() - 1, mario.getPos().getY() - 1);
			std::cout << " ";
		}

	}
}
void Game::pauseGame(const Player& mario,const vector<Barrel>& barrels, const vector<Ghost>& ghosts,const int& climb)
{
	gotoxy(0, GameConfig::HEIGHT + GameConfig::MIN_Y + 1);
	std::cout << "Game Paused";
	char keyPressed = 0;
	mario.draw();
	for (auto& bar : barrels)
		bar.draw();
	for (auto& ghost : ghosts) //draw the ghosts
		ghost.draw();
	while (keyPressed != GameConfig::SPACE)
	{
		if (_kbhit())
			keyPressed = _getch();
	}

	printMarioTrace(mario, climb);

	//print ' ' after the barrels
	printBarrelTraces(barrels);

	//print ' ' after the ghosts
	printGhostsTraces(ghosts);

	//Delete the pause game caption
	gotoxy(0, GameConfig::HEIGHT +GameConfig::MIN_Y + 1);
	std::cout << "            ";
}
void Game:: restart(Player* mario, Point marioStartPos, vector<Barrel>* barrels, int* timetonextbarrel, int* climb, int* jumpsecs, vector<Ghost>* ghosts,const vector<Ghost>& initposesghosts)
{
	//Mario initial position
	mario->setPos({ marioStartPos.getX(), marioStartPos.getY() });
	mario->setDir(GameConfig::STAY);
	mario->setHammer(GameConfig::ARROWKEYS::STAY);
	//Delete all barrels
	barrels->clear();
	//Set the ghosts starting positions
	*ghosts = initposesghosts;

	//Reset other variables
	*timetonextbarrel = *climb = *jumpsecs = 0;
}


void Game::run()
{
	bool gameRunning = false;
	bool gameValid;
	int menuOption = 0;

	map<int,Level> alllevels;
	//gameValid=FileHandler::loadAllFiles(alllevels);

	
	while (true) {
		startMenu();
		menuOption = _getch() - '0';
		int currLvl = 1;
		int lives;
		switch (menuOption)
		{
		case 1:
			gameRunning = true;
			lives = 3;
			break;
		case 8:
			// Show instructions
			showInstructions();
			break;
		case 9:
			// Exit the game
			std::cout << "\nExiting game...\n";
			break;  // Exit the program
		default:
			// Invalid option
			std::cout << "Invalid choice, please try again.\n";
			break;
		}

		while (gameRunning) {
			system("cls");
			ShowConsoleCursor(false);
			//Level currLevel = Level();
			if (currLvl == 1)	
				currLevel->initializeBoard1();
			else
				currLevel->initializeBoard2();


			currLevel->printBoard();
			drawBorders();
			bool finished = false;

			Player mario('@',currLevel->getstartPosMario());
			Player pauline('$',currLevel->getstartPosPauline());
			Player donkeyKong('&',currLevel->getstartPosDonkeyKong());
			Hammer hammer(currLevel->getPosHammer());

			

			donkeyKong.draw();
			pauline.draw();

			//Jump var
			int wPressed = 0;

			//Falling down var
			int descent = 0;

			char keyPressed = 0;
			bool escPressed = false;

			//Ladder vars
			int climb = 0;
			int ladderSteps = 0;
			GameConfig::ARROWKEYS laddermotionprev;
			int indexofCurrLadder = -1;

			//GameSeconds
			int timePlayed = 0;

			//Barrels
			vector<Barrel> barrels;
			LevelSettings currSettings = currLevel->getLevelSettings();
			int timetonextbarrel = 0;
			int currbarrelindex = 0;

			//Ghosts
			vector<Ghost>activeghosts = currLevel->getGhosts(); //Create a copy of the ghosts vector to allow returning to their opening points


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
								if (mario.getDir() == GameConfig::ARROWKEYS::STAY && LeaveLadder(mario.getPos(), currLevel->getLadder(indexofCurrLadder), GameConfig::LEFT, currLevel->getBoardPointer())) //Able to leave the ladder
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
								if (mario.getDir() == GameConfig::ARROWKEYS::STAY && LeaveLadder(mario.getPos(), currLevel->getLadder(indexofCurrLadder), GameConfig::RIGHT, currLevel->getBoardPointer())) //Able to leave the ladder
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
								if (currstate == GameConfig::STAY && (ladderSteps = nearLadder(&mario, currLevel->getLadders(), currLevel->getNumLadders(), GameConfig::UP, &indexofCurrLadder, &climb)) != 0)//Mario is near a ladder
								{
									laddermotionprev = GameConfig::ARROWKEYS::UP;
									mario.setDir(GameConfig::ARROWKEYS::UP);
								}
								else if (descent == 0) //Regular Jump, Not allowed while falling
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
								if (mario.getDir() == GameConfig::STAY && (ladderSteps = nearLadder(&mario, currLevel->getLadders(), currLevel->getNumLadders(), GameConfig::DOWN, &indexofCurrLadder, &climb)) != 0)
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
							pauseGame(mario, barrels,activeghosts,climb);
							break;
						case GameConfig::ESC:
							escPressed = true;
							break;
						case 'p':
						case 'P':
							if (climb == 0&&mario.getHammer()!=GameConfig::STAY) //Mario is able to use the hammer
							{
								GameConfig::ARROWKEYS dirhammer = mario.getHammer();
								if (dirhammer == GameConfig::ARROWKEYS::RIGHT)
								{
									for (int i=0;i<barrels.size();i++)
									{
										if (barrels.at(i).getPos().getX() > mario.getPos().getX()&&mario.getPos().calculateDistance(barrels.at(i).getPos()) <= 2)
										{
											barrels.erase(barrels.begin() + i);
											i--;
										}
									}
									for (int i = 0;i < activeghosts.size();i++)
									{
										if (activeghosts.at(i).getPos().getX() > mario.getPos().getX() && mario.getPos().calculateDistance(activeghosts.at(i).getPos()) <= 2)
										{
											activeghosts.erase(activeghosts.begin() + i);
											i--;
										}
									}
								}
								else //Hammer operates towards left
								{
		                  
									for (int i = 0;i < barrels.size();i++)
									{
										if (barrels.at(i).getPos().getX() < mario.getPos().getX() && mario.getPos().calculateDistance(barrels.at(i).getPos()) <= 2)
										{
											barrels.erase(barrels.begin() + i);
											i--;
										}
									}
									for (int i = 0;i < activeghosts.size();i++)
									{
										if (activeghosts.at(i).getPos().getX() < mario.getPos().getX() && mario.getPos().calculateDistance(activeghosts.at(i).getPos()) <= 2)
										{
											activeghosts.erase(activeghosts.begin() + i);
											i--;
										}
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
						if ((climb == 1 && laddermotionprev == GameConfig::UP)||(climb==(ladderSteps-1)) && laddermotionprev == GameConfig::DOWN)
						{

							int floortoCheck = getFloor(mario.getPos().getY()) + 1;
							if (floortoCheck >= 7) floortoCheck = 7;
							if (laddermotionprev == GameConfig::UP) floortoCheck--;
							char element = currLevel->getBoardValue(floortoCheck, (mario.getPos().getX()) - (GameConfig::MIN_X + 1));
							if (laddermotionprev == GameConfig::UP)
							gotoxy(mario.getPos().getX(), mario.getPos().getY()+1);
							else
								gotoxy(mario.getPos().getX(), mario.getPos().getY());
							if (element != 0)
							{
								switch (element)
								{
								case 1:
									std::cout << '=';
									break;
								case 2:
									std::cout << '>';
									break;
								case 3:
									std::cout << '<';
									break;
								}
							}
						}

						if (mario.getDir() != GameConfig::ARROWKEYS::STAY)
							climb--;
						if (climb == 0)
							mario.setDir(GameConfig::ARROWKEYS::STAY);
					}
					else if (descent > 0) //Falling Down
					{
						if (descent % GameConfig::FLOORDIFF == 0) // checks if the fall shoud stop
						{
							int currFloor = getFloor(mario.getPos().getY());
							int col = mario.getPos().getX() - (GameConfig::MIN_X + 1);
							if (col >= GameConfig::WIDTH - 2)
								col = GameConfig::WIDTH - 2 - 1;
							if (currLevel->getBoardValue(currFloor, col) != 0)
							{
								if (descent >= GameConfig::FLOORDIFF * 3) //Mario fell 3 floors or more
								{
									lives--;
									restart(&mario, currLevel->getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed,&activeghosts,currLevel->getGhosts());
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
						else if (descent % GameConfig::FLOORDIFF == 1 && mario.getDir() != GameConfig::DOWN) //IN Case mario faces a brick while falling diagonally
						{

							int floortoCheck = getFloor(mario.getPos().getY()) + 1;
							char element = currLevel->getBoardValue(floortoCheck, (mario.getPos().getX()) - (GameConfig::MIN_X + 1));
							gotoxy(mario.getPos().getX(), mario.getPos().getY());
							if (element != 0)
							{
								switch (element)
								{
								case 1:
									std::cout << '=';
									break;
								case 2:
									std::cout << '>';
									break;
								case 3:
									std::cout << '<';
									break;
								}
							}
						}
						if (descent != 0)
							descent++;
					}
					else if (wPressed == 0)//Check if mario reached an edge on regular mode,Relevant for non-jumping  mode
					{
						int currFloor = getFloor(mario.getPos().getY());
						if (currLevel->getBoardValue(currFloor, mario.getPos().getX() - (GameConfig::MIN_X + 1)) == 0)
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
							if (currLevel->getBoardValue(currFloor, mario.getPos().getX() - (GameConfig::MIN_X + 1)) != 0)
								mario.setDir(GameConfig::ARROWKEYS::LEFT);
							else
								descent++;
						}

						else if (currstate == GameConfig::DOWNANDRIGHT)
						{
							if (currLevel->getBoardValue(currFloor, mario.getPos().getX() - (GameConfig::MIN_X + 1)) != 0)
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

					if (mario.getHammer() == GameConfig::ARROWKEYS::LEFT)
					{
						mario.setHammer(GameConfig::ARROWKEYS::RIGHT);
						gotoxy(mario.getPos().getX() - 1, mario.getPos().getY() - 1);
						std::cout << 'Q';
					}

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

					if (mario.getHammer() == GameConfig::ARROWKEYS::RIGHT)
					{
						mario.setHammer(GameConfig::ARROWKEYS::LEFT);
						gotoxy(mario.getPos().getX() + 1, mario.getPos().getY() - 1);
						std::cout << 'Q';
					}
					
				}
				if (mario.getPos().getY() >=GameConfig::MIN_Y + GameConfig::HEIGHT - 1) //Mario Fell Down
				{
					lives--;
					restart(&mario, currLevel->getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed, &activeghosts, currLevel->getGhosts());
				}

				//Barrels and Ghosts
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

				if (!barrelsUpdateDirs(&barrels, currLevel->getBoardPointer(), &mario))//Set the exact direction for each barrel and if barrel exploded near mario - restart the game
				{
					lives--;
					restart(&mario, currLevel->getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed, &activeghosts, currLevel->getGhosts());
				}
				ghostsChangeDir(activeghosts);

				for (auto& barel : barrels) //Move the barrels
					barel.move();
				for (auto& ghost : activeghosts) //Move the ghosts
					ghost.move();


				if (marioHitsBarrel(barrels, mario)||marioHitsGhost(activeghosts,mario))
				{
					//mario hit a barrel / ghost
					lives--;
					restart(&mario, currLevel->getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed, &activeghosts, currLevel->getGhosts());
				}

				if (barrelsCheckHits(&barrels, mario)) //delete barrels that share same position (explosion)
				{
					//Mario is near an explosion
					lives--;
					restart(&mario, currLevel->getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed, &activeghosts, currLevel->getGhosts());
				}


				for (int i = 0; i < barrels.size(); i++) //erasing all the barrels that are out of bounds
				{
					if (outOfBounds(barrels[i].getPos()))
					{
						barrels.erase(barrels.begin() + i);
						i--;
					}
						
				}

				mario.move();

				if (marioHitsBarrel(barrels, mario) || marioHitsGhost(activeghosts, mario))
				{
					//mario hit a barrel / ghost
					lives--;
					restart(&mario, currLevel->getstartPosMario(), &barrels, &timetonextbarrel, &climb, &wPressed, &activeghosts, currLevel->getGhosts());
				}
				else if (mario.getPos() == hammer.getPos()&&hammer.getIsVisible()) //mario gets the hammer
				{
					hammer.setIsVisible(false);
					mario.setHammer(mario.getDir());
				}

				currLevel->printLadders();
				printLives(lives,currLevel->getLegendPos());

				if (climb > 0)
					mario.draw(true);
				else
					mario.draw();

				for (auto& barel : barrels) //Draw the barrels
					barel.draw();
				for (auto& ghost : activeghosts) //draw the ghosts
					ghost.draw();

				hammer.draw();

				Sleep(INTERVAL);

				//Game clock is running
				timePlayed += INTERVAL;
				if (timePlayed >= SECOND)
				{
					timePlayed -= SECOND;
					showTime(currLevel->getLegendPos());
				}

				printMarioTrace(mario, climb);
				
				//Print ' ' after the barrels and ghosts
				printBarrelTraces(barrels);
				printGhostsTraces(activeghosts);


				if (mario.getPos() == pauline.getPos())
				{
					finished = true;
					break;
				}
			} while (lives > 0&&escPressed==false);

			if (escPressed == false)
			{
				if (finished)
				{
					gotoxy(0, GameConfig::HEIGHT +GameConfig::MIN_Y + 1);
					std::cout << "Level Won," ;

					//Level up
					currLevel->initLevel();
					currLvl++; 

					if (currLvl == NUMLEVELS + 1) //All levels finished
					{
						system("cls");
						int gamesecs = showTime(currLevel->getLegendPos(),true);

						//Show the time for the whole game
						int minutes = gamesecs / 60;
						int seconds = gamesecs % 60;
						gotoxy(0,GameConfig::MIN_Y);
						std::cout << "Time: ";
						std::cout << setw(2) << setfill('0') << minutes << ":"
							<< setw(2) << setfill('0') << seconds
							<< endl;
						//Show the score based on the game time
						int score = (GameConfig::MAXGAMESECS +20) - gamesecs;
						score > 20 ? std::cout << "Score : " << score<<endl : std::cout << "Score : 20"<<endl;

						gameRunning = false;
						
					}
						
				}
				else
				{
					gotoxy(0, GameConfig::HEIGHT +GameConfig::MIN_Y + 1);
					std::cout << "Failure," ;
					showTime(currLevel->getLegendPos(),true);
					gameRunning = false;
				}
				
				std::cout << "Press any key to continue";
				_getch();
			}
			else //ESC pressed
			{
				gameRunning = false;
				showTime(currLevel->getLegendPos(),true);
			}
				

			
			
		}
	}
}

