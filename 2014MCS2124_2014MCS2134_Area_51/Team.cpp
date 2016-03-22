#ifndef _Header_H
#define _Header_H
#include "header.h"
#endif


class Team
{
	public: 
		int teamID;
		Hero player1;
		Hero player2;
		int nextItem;
		int noOfplayers;
		int castleHealth;
		int currentItemsOnMap[2];
		int itemCoordinates[2][2];
	
	Team()
	{
		castleHealth=1000;
	}
};
