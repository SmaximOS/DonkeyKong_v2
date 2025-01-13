#include "FileHandler.h"
#include <fstream>
bool FileHandler::starts_with(const std::string& str, const std::string& prefix)
{
	return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}
bool FileHandler::ends_with(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
int FileHandler::containValidNumber(const std::string& str)
{
	int mul = 1;
	int sum = 0;
	if (str[6] < '0' || str[6]>'9' || str[7] < '0' || str[7] > '9')
		return -1;

	sum += (str[6] - '0') + (str[7] - '0') * 10;
		return sum;
	
}
int FileHandler::checkFileNameValidity(const string& name)
{
	if (name.size() == FILELEN && starts_with(name, "dkong_") && ends_with(name, ".screen"))
	{
		return containValidNumber(name);
	}
	return 0;
}
bool FileHandler::loadAllFiles(map<int, Level>& alllevels)
{
	bool res=false;

	for (const auto& entry : fs::directory_iterator(fs::current_path()))
	{
		int curLevel;
		if ((curLevel = checkFileNameValidity(entry.path().filename().string())) != 0)
		{
			if (!readFileContent(entry.path().filename().string(), alllevels[curLevel]))
				alllevels.erase(curLevel);
			else
				res = true;
		}
	}

	return res;
}
bool FileHandler:: lookForLegend(const int& ycoor, const string& str, Point* legendPos) 
{ 
	size_t pos = str.find("Lives");
	
	if (pos != string::npos) //Found
	{
		legendPos->setY(GameConfig::MIN_Y+ycoor);
		legendPos->setX(pos);
		return true;
	}

	return false;
}

bool FileHandler:: readFileContent(const string& name,Level& tobuild)
{
	string line;
	bool valid = false;
	bool foundLegend=false;
	ifstream myFile(name, ios::in);
	int currsize,currrow=7,currcol=0,numlines=0;
	int min_x = 0;
	int numline=0;
	Point legendPos;
	vector<Ladder> ladders;

	while (getline(myFile, line) && line.empty() == true);
	
	if (myFile.eof())
		return false;

	//Looking for the upper bound of the board
	while (line[min_x] != ' ' && min_x < line.length() - GameConfig::WIDTH)
		min_x++;

	if (!(min_x < line.length() - GameConfig::WIDTH))
		return false;

	for (int i = 0;i < GameConfig::WIDTH;i++)
	{
		if (line[min_x + i] != 'Q')
			return false;
	}

	foundLegend = lookForLegend(numline, line, &legendPos);

	for (int i = 0; i < GameConfig::HEIGHT; i++)
	{
		myFile.seekg(min_x);
		getline(myFile, line);
		if (line[0] != 'Q'||line.size()<GameConfig::WIDTH||line[GameConfig::WIDTH-1]!='Q')
			return false;

		if ((GameConfig::HEIGHT - i) % GameConfig::FLOORDIFF == 1) //two characters above floor
		{
			if (i == 0) //No Ladders accepted
			{
				for (int j = 1; j < GameConfig::WIDTH - 1; j++)
				{
					if (line[j] != ' ')
						return false;
					
				}
			}
			else
			{
				for (int j = 1; j < GameConfig::WIDTH - 1; j++)
				{
					if (line[j] != ' ')
						return false;

				}
			}
		}








		if (!foundLegend)
			foundLegend = lookForLegend(numline, line, &legendPos);

	}


	return valid;
}