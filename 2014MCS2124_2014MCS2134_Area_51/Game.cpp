#include "header.h"

class Game
{
public:
	int GameType;
	//IP address of teams
	Team team1;
	Team team2;
	int Gametexture[100];

	void initGame(int gameType)
	{
		GameType=gameType;
	}
	
};