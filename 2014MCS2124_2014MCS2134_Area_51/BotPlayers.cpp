#include "header.h"

int first1 = 0;
int first2 = 0;
int first3=0;

void iAmSpawn(Hero *h)
{
	while(h->health<100)
		(h->health)++;
}

void MoveHero(int x_destination, int y_destination, Hero *h)
{
	stack<Point> result;
	if (h->teamNumber == 1)
	{
		if(graph_team1[x_destination][y_destination]!=1)
		{
			result = a_star(graph_team1, h->xPosition, h->yPosition, x_destination, y_destination, 50, 50);
		}
	}
	else if (h->teamNumber == 2)
	{
		if(graph_team2[x_destination][y_destination]!=1)
		{
			result = a_star(graph_team2, h->xPosition, h->yPosition, x_destination, y_destination, 50, 50);
		}
	}
	//printf("A star executed for bot of team number :%d x:%d y:%d Player Number:%d\n",h->teamNumber,x_destination,y_destination,h->playerNumber);
	while (!result.empty())
	{
		//printf("Inside MoveHero\n");
		drawCommonThings();
		  //glutSwapBuffers();
		h->xPosition = (result.top()).x;
		h->yPosition = (result.top()).y;
		//printf("Team:%d Player:%d Result From stack: %d %d \n", h->teamNumber,h->playerNumber,(result.top()).x, (result.top()).y);
		result.pop();

		if(h==mySelf)
		{
			if(!result.empty())
				result.pop();
			if(!result.empty())
				result.pop();
			if(!result.empty())
				result.pop();
			if(!result.empty())
				result.pop();
		}
	}
}

int checkForNearCastle(int team)
{
	int x,y;
	if(team==1)
	{
		//castle 1 : 340,190 and 480,380 ->(140*190)

		if (( gameObj.team2.player1.xPosition>290)
				&& (gameObj.team2.player1.xPosition<530 )
				&& (gameObj.team2.player1.yPosition>140)
				&& ( gameObj.team2.player1.yPosition <430))
		{
			//printf("Inside success check\n");
			return 21;
		}

		if (( gameObj.team2.player2.xPosition>290)
				&& (gameObj.team2.player2.xPosition<530 )
				&& (gameObj.team2.player2.yPosition>140)
				&& ( gameObj.team2.player2.yPosition <430))
		{
			//printf("Inside success check\n");
			return 22;
		}
	}
	else if(team==2)
	{
		    //castle 2 : 930,530 and 1110,650

		if (( gameObj.team1.player1.xPosition>880)
				&& (gameObj.team1.player1.xPosition<1160 )
				&& (gameObj.team1.player1.yPosition>480)
				&& ( gameObj.team1.player1.yPosition <700))
		{
			//printf("Inside success check\n");
			return 11;
		}

		if (( gameObj.team1.player2.xPosition>880)
				&& (gameObj.team1.player2.xPosition<1160 )
				&& (gameObj.team1.player2.yPosition>480)
				&& ( gameObj.team1.player2.yPosition <700))
		{
			//printf("Inside success check\n");
			return 12;
		}
	}
}

int checkInNearRange(int x, int y, int team, int playerNo)
{
	int range=100;
	if (team == 1 && playerNo == 1)
	{
		//printf("inside checkInNearRange 1 1\n");
		if ((x < gameObj.team1.player1.xPosition + range)
				&& (x > gameObj.team1.player1.xPosition - range)
				&& (y < gameObj.team1.player1.yPosition + range)
				&& (y > gameObj.team1.player1.yPosition - range))
		{
			//printf("Inside success check\n");
			return 1;
		}
	}
	else if (team == 1 && playerNo == 2)
	{
		//printf("inside checkInNearRange 1 2\n");
		if ((x < gameObj.team1.player2.xPosition + range)
				&& (x > gameObj.team1.player2.xPosition - range)
				&& (y < gameObj.team1.player2.yPosition + range)
				&& (y > gameObj.team1.player2.yPosition - range))
		{
			//printf("Inside success check\n");
			return 1;
		}
	}
	else if (team == 2 && playerNo == 1)
	{
		//printf("inside checkInNearRange 2 1\n");
		if ((x < gameObj.team2.player1.xPosition + range)
				&& (x > gameObj.team2.player1.xPosition - range)
				&& (y < gameObj.team2.player1.yPosition + range)
				&& (y > gameObj.team2.player1.yPosition - range))
		{
			//printf("Inside success check\n");
			return 1;
		}
	}
	else if (team == 2 && playerNo == 2)
	{
		//printf("inside checkInNearRange 2 2\n");
		if ((x < gameObj.team2.player2.xPosition + range)
				&& (x > gameObj.team2.player2.xPosition - range)
				&& (y < gameObj.team2.player2.yPosition + range)
				&& (y > gameObj.team2.player2.yPosition - range))
		{
			//printf("Inside success check\n");
			return 1;
		}
	}
	return 0;
}

void* Auto_Bot12(void *p)
{
	gameObj.team1.player2.powerID=(gameObj.team1.player2.heroID-1)%4;

	MoveHero(487, 264, &gameObj.team1.player2); // Castle 1
	while (1)
	{
		if(gameObj.team1.player2.health<=0)
		{
			gameObj.team1.player2.health=0;
		}
		if (gameObj.team1.player2.health <20)
		{
			// move to spawn
			MoveHero(63, 812, &gameObj.team1.player2); 
			iAmSpawn(&gameObj.team1.player2);
		}
		if(checkForNearCastle(1)==21)
		{
			MoveHero(gameObj.team2.player1.xPosition, gameObj.team2.player1.yPosition, &gameObj.team1.player2);
			checkValidityofAttackSinglePlayer(gameObj.team2.player1.xPosition,gameObj.team2.player1.yPosition, 1,2);
			continue;
		}
		if(checkForNearCastle(1)==22)
		{
			MoveHero(gameObj.team2.player2.xPosition, gameObj.team2.player2.yPosition, &gameObj.team1.player2);
			checkValidityofAttackSinglePlayer(gameObj.team2.player2.xPosition,gameObj.team2.player2.yPosition, 1,2);
			continue;
		}
		if(checkInNearRange(gameObj.team2.player1.xPosition, gameObj.team2.player1.yPosition,1,2))
		{
			MoveHero(gameObj.team2.player1.xPosition, gameObj.team2.player1.yPosition, &gameObj.team1.player2);
			checkValidityofAttackSinglePlayer(gameObj.team2.player1.xPosition,gameObj.team2.player1.yPosition, 1,2);
		}
		if(checkInNearRange(gameObj.team2.player2.xPosition, gameObj.team2.player2.yPosition,1,2))
		{
			MoveHero(gameObj.team2.player2.xPosition, gameObj.team2.player2.yPosition, &gameObj.team1.player2);
			checkValidityofAttackSinglePlayer(gameObj.team2.player2.xPosition,gameObj.team2.player2.yPosition, 1,2);
		}
		if(checkInNearRange(gameObj.team2.player1.xPosition, gameObj.team2.player1.yPosition,1,1) &&
		   checkInNearRange(gameObj.team2.player2.xPosition, gameObj.team2.player2.yPosition,1,1))
		{
			MoveHero(gameObj.team1.player1.xPosition+5, gameObj.team1.player1.yPosition-5, &gameObj.team1.player2);
		}
		else 
		{
			MoveHero(487, 264, &gameObj.team1.player2);
		}
	}
}

void* Auto_Bot21(void *q)
{
	gameObj.team2.player1.powerID=(gameObj.team2.player1.heroID-1)%4;

	MoveHero(490, 256, &gameObj.team2.player1); // Castle 1
	while (1)
	{
		if(gameObj.team2.player1.health<=0)
		{
			gameObj.team2.player1.health=0;
		}
		if (gameObj.team2.player1.health <20)
		{
			// move to spawn
			MoveHero(1285, 60, &gameObj.team2.player1); // Castle 1
			iAmSpawn(&gameObj.team2.player1);
		}
		if(checkInNearRange(gameObj.team1.player1.xPosition, gameObj.team1.player1.yPosition,2,1))
		{
			MoveHero(gameObj.team1.player1.xPosition, gameObj.team1.player1.yPosition, &gameObj.team2.player1);
			checkValidityofAttackSinglePlayer(gameObj.team1.player1.xPosition,gameObj.team1.player1.yPosition, 2,1);
		}
		if(checkInNearRange(gameObj.team1.player2.xPosition, gameObj.team1.player2.yPosition,2,1))
		{
			MoveHero(gameObj.team1.player2.xPosition, gameObj.team1.player2.yPosition, &gameObj.team2.player1);
			checkValidityofAttackSinglePlayer(gameObj.team1.player2.xPosition,gameObj.team1.player2.yPosition, 2,1);
		}
		if(checkInNearRange(gameObj.team1.player1.xPosition, gameObj.team1.player1.yPosition,2,2) &&
		   checkInNearRange(gameObj.team1.player2.xPosition, gameObj.team1.player2.yPosition,2,2))
		{
			MoveHero(gameObj.team2.player2.xPosition+5, gameObj.team2.player2.yPosition-5, &gameObj.team2.player1);
		}
		else 
		{
			MoveHero(490, 256, &gameObj.team2.player1);
			checkAttackOnCastle(400,200, 2,1);
		}
	}
}

void* Auto_Bot22(void *r)
{
	gameObj.team2.player2.powerID=(gameObj.team2.player2.heroID-1)%4;

	MoveHero(919, 570, &gameObj.team2.player2); // Castle 2
	while (1)
	{
		if(gameObj.team2.player2.health<=0)
		{
			gameObj.team2.player2.health=0;
		}
		if (gameObj.team2.player2.health <20)
		{
			// move to spawn
			MoveHero(1285, 60, &gameObj.team2.player2); 
			iAmSpawn(&gameObj.team2.player2);
		}
		if(checkForNearCastle(2)==11)
		{
			MoveHero(gameObj.team1.player1.xPosition, gameObj.team1.player1.yPosition, &gameObj.team2.player2);
			checkValidityofAttackSinglePlayer(gameObj.team1.player1.xPosition,gameObj.team1.player1.yPosition, 2,2);
			continue;
		}
		if(checkForNearCastle(2)==12)
		{
			MoveHero(gameObj.team1.player2.xPosition, gameObj.team1.player2.yPosition, &gameObj.team2.player2);
			checkValidityofAttackSinglePlayer(gameObj.team1.player2.xPosition,gameObj.team1.player2.yPosition, 2,2);
			continue;
		}
		if(checkInNearRange(gameObj.team1.player1.xPosition, gameObj.team1.player1.yPosition,2,2))
		{
			MoveHero(gameObj.team1.player1.xPosition, gameObj.team1.player1.yPosition, &gameObj.team2.player2);
			checkValidityofAttackSinglePlayer(gameObj.team1.player1.xPosition,gameObj.team1.player1.yPosition, 2,2);
		}
		if(checkInNearRange(gameObj.team1.player2.xPosition, gameObj.team1.player2.yPosition,2,2))
		{
			MoveHero(gameObj.team1.player2.xPosition, gameObj.team1.player2.yPosition, &gameObj.team2.player2);
			checkValidityofAttackSinglePlayer(gameObj.team1.player2.xPosition,gameObj.team1.player2.yPosition, 2,2);
		}
		if(checkInNearRange(gameObj.team1.player1.xPosition, gameObj.team1.player1.yPosition,2,1) &&
		   checkInNearRange(gameObj.team1.player2.xPosition, gameObj.team1.player2.yPosition,2,1))
		{
			MoveHero(gameObj.team2.player1.xPosition+5, gameObj.team2.player1.yPosition-5, &gameObj.team2.player2);
		}
		else 
		{
			MoveHero(919, 570, &gameObj.team2.player2);
		}
	}
}


void startSinglePlayer()
{
	pthread_t BotThread1;
	pthread_t BotThread2;
	pthread_t BotThread3;
	pthread_t display;

	// Set powerID of all bots	
	cout << " Before Creation of bot Thread ";
	
	//Auto Bot for team 2
	int rc = pthread_create(&BotThread1, NULL, Auto_Bot12,NULL);
	if (rc)
	{
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}
	//Auto_Bot(static_cast<void*>(&gameObj.team2.player2));
	printf("Auto Bot 1 Thread created\n");
	rc = pthread_create(&BotThread2, NULL,  Auto_Bot21,NULL);
	
	if (rc)
	{
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}
	//Auto_Bot(static_cast<void*>(&gameObj.team2.player1));
	rc = pthread_create(&BotThread3, NULL, Auto_Bot22,NULL);
	
	if (rc)
	{
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}

	printf("All Threads Created\n");
}

