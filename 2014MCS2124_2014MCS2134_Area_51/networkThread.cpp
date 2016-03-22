#include "header.h"

string clientIPs[3];
int next=0;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

string generatePrimaryServerMessage()
{
	if(gameObj.team1.castleHealth<=0 || (gameObj.team1.player1.health<=0 && gameObj.team1.player2.health<=0))
	{
		// game over
		printf("Game Over\n");
		string msg="gameOver/2/";
		return msg;
	}
	else if(gameObj.team2.castleHealth<=0 || (gameObj.team2.player1.health<=0 && gameObj.team2.player2.health<=0))
	{
		// game over
		printf("Game Over\n");
		string msg="gameOver/1/";
		return msg;
	}


	// Disable Stunning if time is over
	// check for team 1
	if(stun_team1_player1 ==1 || stun_team1_player2==1)
	{
		// team 1 is stun
		if(time(0)>=stun_team1_resume_timer)
		{
			stun_team1_player1=0;
			stun_team1_player2=0;
			stun_team1_resume_timer=-1;
			if(mySelf->teamNumber == 1)
			{
				// server needs to stun himself
				mySelf->stunMySelf=0;
			}
			printf("Resuming stunning of team 1");
		}
	}
	else
	{
		// spawn team 1 player 1
		if(isInSpawn1(gameObj.team1.player1.xPosition,gameObj.team1.player1.yPosition))
		{
			if(gameObj.team1.player1.health <100)
			{
				gameObj.team1.player1.health++;
			}
		}
		// spawn team 1 player 2
		if(isInSpawn1(gameObj.team1.player2.xPosition,gameObj.team1.player2.yPosition))
		{
			if(gameObj.team1.player2.health <100)
			{
				gameObj.team1.player2.health++;
			}
		}
	}
	// check for team 2
	if(stun_team2_player1 ==1 || stun_team2_player2==1)
	{
		// team 2 is stun
		if(time(0)>=stun_team2_resume_timer)
		{
			stun_team2_player1=0;
			stun_team2_player2=0;
			stun_team2_resume_timer=-1;
			if(mySelf->teamNumber == 2)
			{
				// server needs to stun himself
				mySelf->stunMySelf=0;
			}
			printf("Resuming stunning of team 2");
		}
	}
	else
	{
		// spawn team 2 player 1
		if(isInSpawn2(gameObj.team2.player1.xPosition,gameObj.team2.player1.yPosition))
		{
			if(gameObj.team2.player1.health <100)
			{
				gameObj.team2.player1.health++;
			}
		}
		// spawn team 2 player 2
		if(isInSpawn2(gameObj.team2.player2.xPosition,gameObj.team2.player2.yPosition))
		{
			if(gameObj.team2.player2.health <100)
			{
				gameObj.team2.player2.health++;
			}
		}
	}

	// team 1, player 1
	string serverMsg = gameObj.team1.player1.ipAddress + ";"
			+ convertIntToString(gameObj.team1.player1.teamNumber) + ";"
			+ convertIntToString(gameObj.team1.player1.heroID) + ";"
			+ convertIntToString(gameObj.team1.player1.xPosition) + ";"
			+ convertIntToString(gameObj.team1.player1.yPosition) + ";"
			+ convertIntToString(gameObj.team1.player1.health) + ";"
			+ convertIntToString(gameObj.team1.player1.attackSpeed)  + ";"
			+ convertIntToString(gameObj.team1.player1.movementSpeed)+ ";" 			
			+ convertIntToString(stun_team1_player1)+ ";" +"/";
	
	//team 1,player 2
	serverMsg = serverMsg + gameObj.team1.player2.ipAddress + ";"
			+ convertIntToString(gameObj.team1.player2.teamNumber) + ";"
			+ convertIntToString(gameObj.team1.player2.heroID) + ";"
			+ convertIntToString(gameObj.team1.player2.xPosition) + ";"
			+ convertIntToString(gameObj.team1.player2.yPosition) + ";"
			+ convertIntToString(gameObj.team1.player2.health) + ";"
			+ convertIntToString(gameObj.team1.player2.attackSpeed)  + ";"
			+ convertIntToString(gameObj.team1.player2.movementSpeed)+ ";" 			
			+ convertIntToString(stun_team1_player2)+ ";" +"/";
	
	//team 2,player 1
	serverMsg = serverMsg + gameObj.team2.player1.ipAddress + ";"
			+ convertIntToString(gameObj.team2.player1.teamNumber) + ";"
			+ convertIntToString(gameObj.team2.player1.heroID) + ";"
			+ convertIntToString(gameObj.team2.player1.xPosition) + ";"
			+ convertIntToString(gameObj.team2.player1.yPosition) + ";"
			+ convertIntToString(gameObj.team2.player1.health) + ";"
			+ convertIntToString(gameObj.team2.player1.attackSpeed)  + ";"
			+ convertIntToString(gameObj.team2.player1.movementSpeed)+ ";" 			
			+ convertIntToString(stun_team2_player1)+ ";" +"/";
	
	//team 2,player 2
	serverMsg = serverMsg + gameObj.team2.player2.ipAddress + ";"
			+ convertIntToString(gameObj.team2.player2.teamNumber) + ";"
			+ convertIntToString(gameObj.team2.player2.heroID) + ";"
			+ convertIntToString(gameObj.team2.player2.xPosition) + ";"
			+ convertIntToString(gameObj.team2.player2.yPosition) + ";"
			+ convertIntToString(gameObj.team2.player2.health) + ";"
			+ convertIntToString(gameObj.team2.player2.attackSpeed)  + ";"
			+ convertIntToString(gameObj.team2.player2.movementSpeed)+ ";" 			
			+ convertIntToString(stun_team2_player2)+ ";" +"/";
	
	//castle health
	serverMsg = serverMsg + "1;" + convertIntToString(gameObj.team1.castleHealth) +
				";2;" + convertIntToString(gameObj.team2.castleHealth) +";"+"/";

	//Item list
	
	//team 1
	serverMsg = serverMsg + convertIntToString(gameObj.team1.teamID)+";"+
	convertIntToString(gameObj.team1.currentItemsOnMap[0])+";"+
	convertIntToString(gameObj.team1.itemCoordinates[0][0])+";"+
	convertIntToString(gameObj.team1.itemCoordinates[0][1])+";"+
	convertIntToString(gameObj.team1.currentItemsOnMap[1])+";"+
	convertIntToString(gameObj.team1.itemCoordinates[1][0])+";"+
	convertIntToString(gameObj.team1.itemCoordinates[1][1])+";"+"/";

	//team 1
	serverMsg = serverMsg + convertIntToString(gameObj.team2.teamID)+";"+
	convertIntToString(gameObj.team2.currentItemsOnMap[0])+";"+
	convertIntToString(gameObj.team2.itemCoordinates[0][0])+";"+
	convertIntToString(gameObj.team2.itemCoordinates[0][1])+";"+
	convertIntToString(gameObj.team2.currentItemsOnMap[1])+";"+
	convertIntToString(gameObj.team2.itemCoordinates[1][0])+";"+
	convertIntToString(gameObj.team2.itemCoordinates[1][1])+";"+"/";

	return serverMsg;
}

string generateClientMessage()
{
	//client message
	string clientMsg = getMyIPAdress() + ";" + 
					   convertIntToString(x_Coord) + ";"+
					   convertIntToString(y_Coord) + ";"+
					   opponent_player_1_IPAddress + ";"+
					   convertIntToString(opponent_player_1_attacks) + ";"+
					   opponent_player_2_IPAddress + ";"+
					   convertIntToString(opponent_player_2_attacks) + ";"+
					   convertIntToString(attack_on_castle) + ";"+
					   convertIntToString(attack_count_castle) + ";"+
					   convertIntToString(item_used) + ";"+
					   convertIntToString(item_taken)+";"+
					   convertIntToString(iUsedMyMagicalPower)+";"+
					   convertIntToString(mySelf->teamNumber)+";";

    opponent_player_1_attacks=0;
    opponent_player_2_attacks=0;
    attack_count_castle=0;
    item_used=-1;
    item_taken=-1;
    iUsedMyMagicalPower=-1;

	return clientMsg;
}

void applyCheckForHealth()
{
	if(gameObj.team1.player1.health<0)
		gameObj.team1.player1.health=0;
	if(gameObj.team1.player2.health<0)
		gameObj.team1.player2.health=0;
	if(gameObj.team2.player1.health<0)
		gameObj.team2.player1.health=0;
	if(gameObj.team2.player2.health<0)
		gameObj.team2.player2.health=0;

	if(gameObj.team1.castleHealth<0)
		gameObj.team1.castleHealth=0;
	if(gameObj.team2.castleHealth<0)
		gameObj.team2.castleHealth=0;
}

void processMessageRecievedFromClient(string msgFromClient)
{
	//printf("Message From Client : %s\n", msgFromClient.c_str());
	string splittedMessageOfClient[15];
	SplitString(msgFromClient,";",splittedMessageOfClient);

	//checking for attack on castle
	if(splittedMessageOfClient[7]=="1")
	{
		gameObj.team1.castleHealth = gameObj.team1.castleHealth - atoi(splittedMessageOfClient[8].c_str());
	}
	else if(splittedMessageOfClient[7]=="2")
	{
		gameObj.team2.castleHealth = gameObj.team2.castleHealth - atoi(splittedMessageOfClient[8].c_str());
	}

	if(gameObj.team1.player1.ipAddress == splittedMessageOfClient[0])
	{
		gameObj.team1.player1.xPosition=atoi(splittedMessageOfClient[1].c_str());
		gameObj.team1.player1.yPosition=atoi(splittedMessageOfClient[2].c_str());
	}
	else if(gameObj.team1.player2.ipAddress == splittedMessageOfClient[0])
	{
		gameObj.team1.player2.xPosition=atoi(splittedMessageOfClient[1].c_str());
		gameObj.team1.player2.yPosition=atoi(splittedMessageOfClient[2].c_str());
	}
	else if(gameObj.team2.player1.ipAddress == splittedMessageOfClient[0])
	{
		gameObj.team2.player1.xPosition=atoi(splittedMessageOfClient[1].c_str());
		gameObj.team2.player1.yPosition=atoi(splittedMessageOfClient[2].c_str());
	}
	else if(gameObj.team2.player2.ipAddress == splittedMessageOfClient[0])
	{
		gameObj.team2.player2.xPosition=atoi(splittedMessageOfClient[1].c_str());
		gameObj.team2.player2.yPosition=atoi(splittedMessageOfClient[2].c_str());
	}

	if(splittedMessageOfClient[3]==gameObj.team1.player1.ipAddress)
	{
		gameObj.team1.player1.health=gameObj.team1.player1.health - atoi(splittedMessageOfClient[4].c_str());
	}
	else if(splittedMessageOfClient[3]==gameObj.team1.player2.ipAddress)
	{
		gameObj.team1.player2.health=gameObj.team1.player2.health - atoi(splittedMessageOfClient[4].c_str());
	}
	else if(splittedMessageOfClient[3]==gameObj.team2.player1.ipAddress)
	{
		gameObj.team2.player1.health=gameObj.team2.player1.health - atoi(splittedMessageOfClient[4].c_str());
	}
	else if(splittedMessageOfClient[3]==gameObj.team2.player2.ipAddress)
	{
		gameObj.team2.player2.health=gameObj.team2.player2.health - atoi(splittedMessageOfClient[4].c_str());
	}


	if(splittedMessageOfClient[5]==gameObj.team1.player1.ipAddress)
	{
		gameObj.team1.player1.health=gameObj.team1.player1.health - atoi(splittedMessageOfClient[6].c_str());
	}
	else if(splittedMessageOfClient[5]==gameObj.team1.player2.ipAddress)
	{
		gameObj.team1.player2.health=gameObj.team1.player2.health - atoi(splittedMessageOfClient[6].c_str());
	}
	else if(splittedMessageOfClient[5]==gameObj.team2.player1.ipAddress)
	{
		gameObj.team2.player1.health=gameObj.team2.player1.health - atoi(splittedMessageOfClient[6].c_str());
	}
	else if(splittedMessageOfClient[5]==gameObj.team2.player2.ipAddress)
	{
		gameObj.team2.player2.health=gameObj.team2.player2.health - atoi(splittedMessageOfClient[6].c_str());
	}


	//Processing Items
	if(splittedMessageOfClient[9]!="-1")
	{
		//check for item 0 -> attack_speed_item

		printf("Server says Item used by client if of location:%s\n",splittedMessageOfClient[10].c_str() );

		if(splittedMessageOfClient[9]=="0")
		{

			if(gameObj.team1.player1.ipAddress == splittedMessageOfClient[0])
			{
				//printf("Item 0 consumed : before -> %d \n",gameObj.team1.nextItem);
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
				//printf("Item 0 consumed : After -> %d \n",gameObj.team1.nextItem);
			}
			else if(gameObj.team1.player2.ipAddress == splittedMessageOfClient[0])
			{
				//printf("Item 0 consumed : before -> %d \n",gameObj.team1.nextItem);
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
				//printf("Item 0 consumed : before -> %d \n",gameObj.team1.nextItem);
			}
			else if(gameObj.team2.player1.ipAddress == splittedMessageOfClient[0])
			{
				//printf("Item 0 consumed : before -> %d \n",gameObj.team2.nextItem);
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
				//printf("Item 0 consumed : before -> %d \n",gameObj.team2.nextItem);
			}
			else if(gameObj.team2.player2.ipAddress == splittedMessageOfClient[0])
			{
				//printf("Item 0 consumed : before -> %d \n",gameObj.team2.nextItem);
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
				//printf("Item 0 consumed : before -> %d \n",gameObj.team2.nextItem);
			}
	    }	

		//check for item 1 -> damage_item

		if(splittedMessageOfClient[9]=="1")
		{
			if(gameObj.team1.player1.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team2.castleHealth=gameObj.team2.castleHealth - 25;
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team1.player2.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team2.castleHealth=gameObj.team2.castleHealth - 25;
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team2.player1.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team1.castleHealth=gameObj.team2.castleHealth - 25;
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team2.player2.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team1.castleHealth=gameObj.team2.castleHealth - 25;
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
			}
	    }	

		//check for item 2 -> health_item

		if(splittedMessageOfClient[9]=="2")
		{
			if(gameObj.team1.player1.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team1.player1.health = gameObj.team1.player1.health + 25;
				if(gameObj.team1.player1.health > 100)
					gameObj.team1.player1.health=100;
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team1.player2.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team1.player2.health = gameObj.team1.player2.health + 25;
				if(gameObj.team1.player2.health > 100)
					gameObj.team1.player2.health=100;
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team2.player1.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team2.player1.health = gameObj.team2.player1.health + 25;

				if(gameObj.team2.player1.health > 100)
					gameObj.team2.player1.health=100;
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team2.player2.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team2.player2.health = gameObj.team2.player2.health + 25;
				if(gameObj.team2.player1.health > 100)
					gameObj.team2.player1.health=100;
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
			}
	    }	
		//check for item 3 -> speed_item

		if(splittedMessageOfClient[9]=="3")
		{
			if(gameObj.team1.player1.ipAddress == splittedMessageOfClient[0])
			{
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team1.player2.ipAddress == splittedMessageOfClient[0])
			{
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team2.player1.ipAddress == splittedMessageOfClient[0])
			{
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team2.player2.ipAddress == splittedMessageOfClient[0])
			{
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
			}
	    }	
		//check for item 4 -> temple_healer

		if(splittedMessageOfClient[9]=="4")
		{
			if(gameObj.team1.player1.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team1.castleHealth=gameObj.team2.castleHealth + 50;
				if(gameObj.team1.castleHealth>1000)
				{
					gameObj.team1.castleHealth=1000;
				}
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team1.player2.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team1.castleHealth=gameObj.team2.castleHealth + 50;
				if(gameObj.team1.castleHealth>1000)
				{
					gameObj.team1.castleHealth=1000;
				}
				generateItemForTeam1(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team2.player1.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team2.castleHealth=gameObj.team2.castleHealth + 50;
				if(gameObj.team2.castleHealth>1000)
				{
					gameObj.team2.castleHealth=1000;
				}
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
			}
			else if(gameObj.team2.player2.ipAddress == splittedMessageOfClient[0])
			{
				gameObj.team2.castleHealth=gameObj.team2.castleHealth + 50;
				if(gameObj.team2.castleHealth>1000)
				{
					gameObj.team2.castleHealth=1000;
				}
				generateItemForTeam2(atoi(splittedMessageOfClient[10].c_str()));
			}
	    }	
	}

	applyCheckForHealth();

	// Processing magical power if used

	if(splittedMessageOfClient[11]!="-1")
	{
		// Check whether he is allowed to use its magical power or not

		if(disableTeam == atoi(splittedMessageOfClient[12].c_str()))
		{
			// check for time
			if(time(0) < disableResumeTimer)
			{
				printf("Power of %d is disabled..\n",disableTeam);
				return;
			}
			else if(time(0) >= disableResumeTimer)
			{
				disableResumeTimer=-1;
				disableTeam=-1;
				printf("Resuming Magical Power for Team %d \n",splittedMessageOfClient[12].c_str());
			}
		}

		// Magical Power is used
		if(splittedMessageOfClient[11]=="0")
		{
			// Disabler
			if(splittedMessageOfClient[12]=="1")
			{
				// player from team 1 has asked to disable magical power of team 2
				printf("Team 1 player used Magical Power of Disabler\n");
				disableTeam=2;
				disableResumeTimer=time(0)+DISAMBLER_TIME;
			}
			else if(splittedMessageOfClient[12]=="2")
			{
				// player from team 2 has asked to disable magical power of team 1
				printf("Team 2 player used Magical Power of Disabler\n");
				disableTeam=1;
				disableResumeTimer=time(0)+DISAMBLER_TIME;
			}
		}
		else if(splittedMessageOfClient[11]=="1")
		{
			// slower
			if(splittedMessageOfClient[12]=="1")  // Recieved msg from client of team Number 1
			{
				gameObj.team2.player1.attackSpeed = ATTACK_SPEED_SLOW;
				gameObj.team2.player1.movementSpeed = MOVEMENT_SPEED_SLOW;
				gameObj.team2.player2.attackSpeed = ATTACK_SPEED_SLOW;
				gameObj.team2.player2.movementSpeed = MOVEMENT_SPEED_SLOW;
				printf("Team 1 player used Magical Power of slower\n");
			}
			else if(splittedMessageOfClient[12]=="2")  // Recieved msg from client of team Number 2
			{
				gameObj.team1.player1.attackSpeed = ATTACK_SPEED_SLOW;
				gameObj.team1.player1.movementSpeed = MOVEMENT_SPEED_SLOW;
				gameObj.team1.player2.attackSpeed = ATTACK_SPEED_SLOW;
				gameObj.team1.player2.movementSpeed = MOVEMENT_SPEED_SLOW;
				printf("Team 2 player used Magical Power of slower\n");
			}
		}
		else if(splittedMessageOfClient[11]=="2")
		{
			// Burst Damage
			if(splittedMessageOfClient[12]=="1")  // Recieved msg from client of team Number 1
			{
				// decrease health of oppponent
				gameObj.team2.player1.health = gameObj.team2.player1.health - 25;
				gameObj.team2.player2.health = gameObj.team2.player2.health - 25;
				gameObj.team2.castleHealth = gameObj.team2.castleHealth - 25;

				// decrease its attack speed and movement speed -> TOPUTORNOT
				if(gameObj.team1.player1.ipAddress == splittedMessageOfClient[0])
				{
					gameObj.team1.player1.attackSpeed = ATTACK_SPEED_SLOW;
					gameObj.team1.player1.movementSpeed = MOVEMENT_SPEED_SLOW;
					printf("Team 1 player used Magical Power of Burst Damage\n");
				}
				else if(gameObj.team1.player2.ipAddress == splittedMessageOfClient[0])
				{
					gameObj.team1.player2.attackSpeed = ATTACK_SPEED_SLOW;
					gameObj.team1.player2.movementSpeed = MOVEMENT_SPEED_SLOW;
					printf("Team 1 player used Magical Power of Burst Damage\n");
				}
			}
			else if(splittedMessageOfClient[12]=="2")  // Recieved msg from client of team Number 2
			{
				// decrease health of oppponent
				gameObj.team1.player1.health = gameObj.team1.player1.health - 25;
				gameObj.team1.player2.health = gameObj.team1.player2.health - 25;
				gameObj.team1.castleHealth = gameObj.team1.castleHealth - 25;

				// decrease its attack speed and movement speed -> TOPUTORNOT
				if(gameObj.team2.player1.ipAddress == splittedMessageOfClient[0])
				{
					gameObj.team2.player1.attackSpeed = ATTACK_SPEED_SLOW;
					gameObj.team2.player1.movementSpeed = MOVEMENT_SPEED_SLOW;
					printf("Team 2 player used Magical Power of Burst Damage\n");
				}
				else if(gameObj.team2.player2.ipAddress == splittedMessageOfClient[0])
				{
					gameObj.team2.player2.attackSpeed = ATTACK_SPEED_SLOW;
					gameObj.team2.player2.movementSpeed = MOVEMENT_SPEED_SLOW;
					printf("Team 2 player used Magical Power of Burst Damage\n");
				}
			}
		}
		else if(splittedMessageOfClient[11]=="3")
		{
			// Stunner
			if(splittedMessageOfClient[12]=="1")
			{
				// someone from team 1 will stun team 2
				stun_team2_player1=1;
				stun_team2_player2=1;
				if(mySelf->teamNumber == 2)
				{
					// server needs to stun himself
					mySelf->stunMySelf=1;
				}
				stun_team2_resume_timer=time(0)+STUN_TIME;
				printf("Team 1 player used Magical Power of Stunner\n");
			}
			else if(splittedMessageOfClient[12]=="2")
			{
				// someone from team 2 will stun team 1
				stun_team1_player1=1;
				stun_team1_player2=1;
				if(mySelf->teamNumber == 1)
				{
					// server needs to stun himself
					mySelf->stunMySelf=1;
				}
				stun_team1_resume_timer=time(0)+STUN_TIME;
				printf("Team 2 player used Magical Power of Stunner\n");
			}
		}
	}
	applyCheckForHealth();
}

int processServerMessage(string originalMessage)
{
	//printf("Inside processServerMessage 1\n");

	//printf("Processing Message: %s\n",originalMessage.c_str());

	myIPAddress=getMyIPAdress();

	string splittedMessagePrimary[8];

	SplitString(originalMessage,"/",splittedMessagePrimary);

	//printf("Inside processServerMessage 2\n");

	//Process splittedMessagePrimary[0] -> team1.player1

	string splitTeam1Player1[10];
	SplitString(splittedMessagePrimary[0],";",splitTeam1Player1);

	//printf("Inside processServerMessage 3\n");

	if(splitTeam1Player1[0]!=myIPAddress)
	{
		gameObj.team1.player1.xPosition = atoi(splitTeam1Player1[3].c_str());
		gameObj.team1.player1.yPosition = atoi(splitTeam1Player1[4].c_str());
		gameObj.team1.player1.health = atoi(splitTeam1Player1[5].c_str());
		gameObj.team1.player1.attackSpeed = atoi(splitTeam1Player1[6].c_str());
		gameObj.team1.player1.movementSpeed = atoi(splitTeam1Player1[7].c_str());
		gameObj.team1.player1.stunMySelf=  atoi(splitTeam1Player1[8].c_str());
	}
	else if (splitTeam1Player1[0]==myIPAddress)
	{
		gameObj.team1.player1.health = atoi(splitTeam1Player1[5].c_str());

		if(gameObj.team1.player1.health<=0)
		{
			mySelf->dead=1;
			x_Mouse=x_Coord=70;
			y_Mouse=y_Coord=850;
			deadTimer=time(0)+5;
		}

		gameObj.team1.player1.attackSpeed = atoi(splitTeam1Player1[6].c_str());
		gameObj.team1.player1.movementSpeed = atoi(splitTeam1Player1[7].c_str());
		gameObj.team1.player1.stunMySelf=  atoi(splitTeam1Player1[8].c_str());
	}

	//Process splittedMessagePrimary[1] -> team1.player2

	string splitTeam1Player2[10];
	SplitString(splittedMessagePrimary[1],";",splitTeam1Player2);

	//printf("Inside processServerMessage 4\n");

	if(splitTeam1Player2[0]!=myIPAddress)
	{
		gameObj.team1.player2.xPosition = atoi(splitTeam1Player2[3].c_str());
		gameObj.team1.player2.yPosition = atoi(splitTeam1Player2[4].c_str());
		gameObj.team1.player2.health = atoi(splitTeam1Player2[5].c_str());
		gameObj.team1.player2.attackSpeed = atoi(splitTeam1Player2[6].c_str());
		gameObj.team1.player2.movementSpeed = atoi(splitTeam1Player2[7].c_str());
		gameObj.team1.player2.stunMySelf= atoi(splitTeam1Player2[8].c_str());
	}
	else if (splitTeam1Player2[0]==myIPAddress)
	{
		gameObj.team1.player2.health = atoi(splitTeam1Player2[5].c_str());
		if(gameObj.team1.player2.health<=0)
		{
			mySelf->dead=1;
			x_Mouse= x_Coord=50;
			y_Mouse= y_Coord=830;

			deadTimer=time(0)+5;
		}		
		gameObj.team1.player2.attackSpeed = atoi(splitTeam1Player2[6].c_str());
		gameObj.team1.player2.movementSpeed = atoi(splitTeam1Player2[7].c_str());
		gameObj.team1.player2.stunMySelf= atoi(splitTeam1Player2[8].c_str());
	}


	//Process splittedMessagePrimary[2] -> team2.player1

	string splitTeam2Player1[10];
	SplitString(splittedMessagePrimary[2],";",splitTeam2Player1);

	if(splitTeam2Player1[0]!=myIPAddress)
	{
		gameObj.team2.player1.xPosition = atoi(splitTeam2Player1[3].c_str());
		gameObj.team2.player1.yPosition = atoi(splitTeam2Player1[4].c_str());
		gameObj.team2.player1.health = atoi(splitTeam2Player1[5].c_str());
		gameObj.team2.player1.attackSpeed = atoi(splitTeam2Player1[6].c_str());
		gameObj.team2.player1.movementSpeed = atoi(splitTeam2Player1[7].c_str());
		gameObj.team2.player1.stunMySelf= atoi(splitTeam2Player1[8].c_str());
	}
	else if (splitTeam2Player1[0]==myIPAddress)
	{
		gameObj.team2.player1.health = atoi(splitTeam2Player1[5].c_str());
		if(gameObj.team2.player1.health<=0)
		{
			mySelf->dead=1;
			x_Mouse= x_Coord=1350;
			y_Mouse= y_Coord=75;
			deadTimer=time(0)+5;
		}		
		gameObj.team2.player1.attackSpeed = atoi(splitTeam2Player1[6].c_str());
		gameObj.team2.player1.movementSpeed = atoi(splitTeam2Player1[7].c_str());
		gameObj.team2.player1.stunMySelf= atoi(splitTeam2Player1[8].c_str());
	}


	//Process splittedMessagePrimary[3] -> team2.player2

	string splitTeam2Player2[10];
	SplitString(splittedMessagePrimary[3],";",splitTeam2Player2);

	if(splitTeam2Player2[0]!=myIPAddress)
	{
		gameObj.team2.player2.xPosition = atoi(splitTeam2Player2[3].c_str());
		gameObj.team2.player2.yPosition = atoi(splitTeam2Player2[4].c_str());
		gameObj.team2.player2.health = atoi(splitTeam2Player2[5].c_str());
		gameObj.team2.player2.attackSpeed = atoi(splitTeam2Player2[6].c_str());
		gameObj.team2.player2.movementSpeed = atoi(splitTeam2Player2[7].c_str());
		gameObj.team2.player2.stunMySelf= atoi(splitTeam2Player2[8].c_str());
	}
	else if (splitTeam2Player2[0]==myIPAddress)
	{
		gameObj.team2.player2.health = atoi(splitTeam2Player2[5].c_str());
		if(gameObj.team2.player2.health<=0)
		{
			mySelf-> dead=1;
			x_Mouse=x_Coord=1315;
			y_Mouse=y_Coord=50;
			deadTimer=time(0)+5;
		}		
		gameObj.team2.player2.attackSpeed = atoi(splitTeam2Player2[6].c_str());
		gameObj.team2.player2.movementSpeed = atoi(splitTeam2Player2[7].c_str());
		gameObj.team2.player2.stunMySelf= atoi(splitTeam2Player2[8].c_str());
	}

	//Process splittedMessagePrimary[4] -> castle health

	string infoOfCastleHealth[5];
	SplitString(splittedMessagePrimary[4],";",infoOfCastleHealth);
	gameObj.team1.castleHealth = atoi(infoOfCastleHealth[1].c_str());
	gameObj.team2.castleHealth = atoi(infoOfCastleHealth[3].c_str());

	//Process splittedMessagePrimary[5] & [6] -> info of items -> team1,item2

	string itemInfoTeam1[10];
	string itemInfoTeam2[10];
	SplitString(splittedMessagePrimary[5],";",itemInfoTeam1);
	SplitString(splittedMessagePrimary[6],";",itemInfoTeam2);

	if(gameObj.team1.player1.ipAddress == myIPAddress || gameObj.team1.player2.ipAddress == myIPAddress)
	{
		// I am from team 1 , use itemInfoTeam1

		//Item 1
		gameObj.team1.currentItemsOnMap[0] = atoi(itemInfoTeam1[1].c_str());
		gameObj.team1.itemCoordinates[0][0] = atoi(itemInfoTeam1[2].c_str());
		gameObj.team1.itemCoordinates[0][1] = atoi(itemInfoTeam1[3].c_str());

		//Item 2
		gameObj.team1.currentItemsOnMap[1] = atoi(itemInfoTeam1[4].c_str());
		gameObj.team1.itemCoordinates[1][0] = atoi(itemInfoTeam1[5].c_str());
		gameObj.team1.itemCoordinates[1][1] = atoi(itemInfoTeam1[6].c_str());	

		/*printf("My Items : Item 1 - Number %d x:%d y:%d Item 2 - Number %d x:%d y:%d \n",gameObj.team1.currentItemsOnMap[0],
		 gameObj.team1.itemCoordinates[0][0],gameObj.team1.itemCoordinates[0][1],gameObj.team1.currentItemsOnMap[1],
		 gameObj.team1.itemCoordinates[1][0],gameObj.team1.itemCoordinates[1][1]);	*/		
	}

	if(gameObj.team2.player1.ipAddress == myIPAddress || gameObj.team2.player2.ipAddress == myIPAddress)
	{
		// I am from team 2 , use itemInfoTeam2

		//Item 1
		gameObj.team2.currentItemsOnMap[0] = atoi(itemInfoTeam2[1].c_str());
		gameObj.team2.itemCoordinates[0][0] = atoi(itemInfoTeam2[2].c_str());
		gameObj.team2.itemCoordinates[0][1] = atoi(itemInfoTeam2[3].c_str());

		//Item 2
		gameObj.team2.currentItemsOnMap[1] = atoi(itemInfoTeam2[4].c_str());
		gameObj.team2.itemCoordinates[1][0] = atoi(itemInfoTeam2[5].c_str());
		gameObj.team2.itemCoordinates[1][1] = atoi(itemInfoTeam2[6].c_str());	

		/*printf("My Items : Item 1 - Number %d x:%d y:%d Item 2 - Number %d x:%d y:%d \n",gameObj.team2.currentItemsOnMap[0],
		 gameObj.team2.itemCoordinates[0][0],gameObj.team2.itemCoordinates[0][1],gameObj.team2.currentItemsOnMap[1],
		 gameObj.team2.itemCoordinates[1][0],gameObj.team2.itemCoordinates[1][1]);	*/				
	}

	//printf("Inside processServerMessage End\n");
}

void loadheroTextures(int teamnumber,int playernumber,int h_no)
{
	string image_path = heros_path +convertIntToString(h_no) + "/image_";
    for (int i = 1; i <= 8; i++)
    {
        string hero_img = image_path + convertIntToString(i) + ".png";
        printf("%s\n", hero_img.c_str());
        if(teamnumber==1 && playernumber==1)
        {
        	funLoadGLTextures(hero_img.c_str(),
            gameObj.team1.player1.texture, i - 1);
        	// gameObj.team1.player1.xPosition=100;
       	}
		else if(teamnumber==1 && playernumber==2)
        {
        	funLoadGLTextures(hero_img.c_str(),
            gameObj.team1.player2.texture, i - 1);
        	//gameObj.team1.player2.xPosition=500;
     	}
		else if(teamnumber==2 && playernumber==1)
        {
        	funLoadGLTextures(hero_img.c_str(),
            gameObj.team2.player1.texture, i - 1);
        	//gameObj.team2.player1.xPosition=300;
      	}
        printf("Texture of hero set for :: team number %d playernumber %d h_no %d xPosition::\n",teamnumber,playernumber,h_no);
    }
}

int processClientIPandPort(string joinMessage)
{
	if (noOfClients < 4)
	{
		string result[15];
		SplitString(joinMessage, ";", result);
		printf("Message splitted successfully\n");

		int teamNumber = atoi(result[2].c_str());
		int heroNumber = atoi(result[3].c_str());
		printf("%s  %d\n",joinMessage.c_str(),heroNumber);
		if (teamNumber == 1)
		{
			if (gameObj.team1.noOfplayers == 0)
			{
				printf("..............Texture of hero set for :: team number %d playernumber %d h_no %d xPosition::\n",teamNumber,1,heroNumber);
				gameObj.team1.player1.ipAddress = result[1];
				gameObj.team1.player1.heroID = heroNumber;
				gameObj.team1.player1.playerNumber = 1;
				gameObj.team1.player1.xPosition = 70;
				gameObj.team1.player1.yPosition = 850;
				gameObj.team1.player1.teamNumber=1;
				gameObj.team1.player1.health=100;
				gameObj.team1.player1.attackSpeed= ATTACK_SPEED_MEDIUM;
				gameObj.team1.player1.movementSpeed=MOVEMENT_SPEED_MEDIUM;
				gameObj.team1.player1.powerCount=POWER_COUNT;

				
				//loadheroTextures(teamNumber,1,heroNumber);
				gameObj.team1.noOfplayers++;

				printf("IP of team1 player1 set to %s\n", gameObj.team1.player1.ipAddress.c_str());

				printf("Initialised player 1 of Team1 hero number:: %d\n",heroNumber);

				if(gameObj.team1.player1.ipAddress == myIPAddress)
				{
					//Its Me
					mySelf = &gameObj.team1.player1;
					x_Coord=gameObj.team1.player1.xPosition;
					y_Coord=gameObj.team1.player1.yPosition;
				}
				//noOfClients++;
			}
			else if (gameObj.team1.noOfplayers == 1)
			{
				printf("..........Texture of hero set for :: team number %d playernumber %d h_no %d xPosition::\n",teamNumber,1,heroNumber);
				gameObj.team1.player2.ipAddress = result[1];
				gameObj.team1.player2.heroID = heroNumber;
				gameObj.team1.player2.playerNumber = 2;
				gameObj.team1.player2.xPosition = 50;
				gameObj.team1.player2.yPosition = 830;
				gameObj.team1.player2.teamNumber=1;
				gameObj.team1.player2.health=100;
				gameObj.team1.player2.attackSpeed=ATTACK_SPEED_MEDIUM;
				gameObj.team1.player2.movementSpeed=MOVEMENT_SPEED_MEDIUM;
				gameObj.team1.player2.powerCount=POWER_COUNT;
				//loadheroTextures(teamNumber,2,heroNumber);
				gameObj.team1.noOfplayers++;
				printf("Initialised player 2 of Team1hero number:: %d\n",heroNumber);

				printf("IP of team1 player2 set to %s\n", gameObj.team1.player2.ipAddress.c_str());

				if(gameObj.team1.player2.ipAddress == myIPAddress)
				{
					//Its Me
					mySelf = &gameObj.team1.player2;
					x_Coord=gameObj.team1.player2.xPosition;
					y_Coord=gameObj.team1.player2.yPosition;
				}
				//noOfClients++;
			}
			else
			{
				printf("Team size exceeded for Team 1");
				return 0;
			}

		}
		else if (teamNumber == 2)
		{
			if (gameObj.team2.noOfplayers == 0)
			{printf(".............Texture of hero set for :: team number %d playernumber %d h_no %d xPosition::\n",teamNumber,1,heroNumber);
				gameObj.team2.player1.ipAddress = result[1];
				gameObj.team2.player1.heroID = heroNumber;
				gameObj.team2.player1.playerNumber = 1;
				gameObj.team2.player1.xPosition = 1350;
				gameObj.team2.player1.yPosition = 75;
				gameObj.team2.player1.teamNumber=2;
				gameObj.team2.player1.health=100;				
				gameObj.team2.player1.attackSpeed=ATTACK_SPEED_MEDIUM;
				gameObj.team2.player1.movementSpeed=MOVEMENT_SPEED_MEDIUM;
				gameObj.team2.player1.powerCount=POWER_COUNT;
				//loadheroTextures(teamNumber,1,heroNumber);
				gameObj.team2.noOfplayers++;
				printf("Initialised player 1 of Team2 hero number:: %d\n",heroNumber);

				printf("IP of team2 player1 set to %s\n", gameObj.team2.player1.ipAddress.c_str());

				if(gameObj.team2.player1.ipAddress == myIPAddress)
				{
					//Its Me
					mySelf = &gameObj.team2.player1;
					x_Coord=gameObj.team2.player1.xPosition;
					y_Coord=gameObj.team2.player1.yPosition;
				}
				//noOfClients++;
			}
			else if (gameObj.team2.noOfplayers == 1)
			{printf("...............Texture of hero set for :: team number %d playernumber %d h_no %d xPosition::\n",teamNumber,1,heroNumber);
				gameObj.team2.player2.ipAddress = result[1];
				gameObj.team2.player2.heroID = heroNumber;
				gameObj.team2.player2.playerNumber = 2;
				gameObj.team2.player2.xPosition = 1315;
				gameObj.team2.player2.yPosition = 50;
				gameObj.team2.player2.health=100;
				gameObj.team2.player2.teamNumber=2;				
				gameObj.team2.player2.attackSpeed=ATTACK_SPEED_MEDIUM;
				gameObj.team2.player2.movementSpeed=MOVEMENT_SPEED_MEDIUM;
				gameObj.team2.player2.powerCount=POWER_COUNT;
				//loadheroTextures(teamNumber,2,heroNumber);
				gameObj.team2.noOfplayers++;
				printf("Initialised player 2 of Team2 hero number:: %d\n",heroNumber);

				printf("IP of team2 player2 set to %s\n", gameObj.team2.player2.ipAddress.c_str());

				if(gameObj.team2.player2.ipAddress == myIPAddress)
				{
					//Its Me
					mySelf = &gameObj.team2.player2;
					x_Coord=gameObj.team2.player2.xPosition;
					y_Coord=gameObj.team2.player2.yPosition;
				}
				//noOfClients++;
			}
			else
			{
				printf("Team size exceeded for Team 2");
				return 0;
			}

		}
		else
		{
			printf("Invalid Team Number");
			return 0;
		}
		return 1;
	}
	printf("Number of clients exceeded");
	return 0;
}

void setMyParameters()
{
	string myIPAddress=getMyIPAdress();

	if(gameObj.team1.player1.ipAddress == myIPAddress)
	{
		gameObj.team1.player1.xPosition=x_Coord;
		gameObj.team1.player1.yPosition=y_Coord;
	}
	else if(gameObj.team1.player2.ipAddress == myIPAddress)
	{
		gameObj.team1.player2.xPosition=x_Coord;
		gameObj.team1.player2.yPosition=y_Coord;
	}
	else if(gameObj.team2.player1.ipAddress == myIPAddress)
	{
		gameObj.team2.player1.xPosition=x_Coord;
		gameObj.team2.player1.yPosition=y_Coord;
	}
	else if(gameObj.team2.player2.ipAddress == myIPAddress)
	{
		gameObj.team2.player1.xPosition=x_Coord;
		gameObj.team2.player1.xPosition=y_Coord;
	}
}

#include "MessageProcessing.cpp"
int listenGameRequest()
{
	int sockfd , newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr , cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("ERROR opening socket\n");
		return 0;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(server_port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("ERROR on binding\n");
		return 0;
	}
	listen(sockfd, 10);
	while (noOfClients != 3)
	{
		printf("Inside while statement with noOfClients %d \n",noOfClients);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
		{
			printf("ERROR on accept\n");
			return 0;
		}
		printf("Node connected\n");
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0)
		{
			printf("ERROR reading from socket\n");
			return 0;
		}
		printf("Here is the message received from Joining threads: %s\n", buffer);

		string message_recieved = string(buffer);
		//processClientIPandPort(message_recieved);

		string send_ack = "Thank You for Joining ... " + message_recieved;

		n = write(newsockfd, send_ack.c_str(), 100);
		if (n < 0)
		{
			printf("ERROR writing to socket\n");
			return 0;
		}
		
		string rMsg[4];
		SplitString(message_recieved,";",rMsg);
		clientIPs[noOfClients]=rMsg[1];
		processClientIPandPort(message_recieved);
		noOfClients++;
		close(newsockfd);
	}
	printf("Tired of listening to clients...while exited\n");
	close(sockfd);
	return 1;
}

void sendJoinMessageToAll()
{
	int test=1;
	printf("Inside sendJoinMessageToAll\n");

	int myParameterSet=0;

	while(1)
	{
		int sockfd, portno, n;

	    struct sockaddr_in serv_addr;
	    struct hostent *server;

	    char buffer[1000];
	    
	    portno = 4001;

	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0) 
	        error("ERROR opening socket");
	    server = gethostbyname(clientIPs[next].c_str());

	    //printf("Client : %s\n", clientIPs[next].c_str());
	    next=(next+1);

	    if(next == 3)
	    {
	    	next=0;

	    	//printf("Changing my parameteres\n");
	    	//setMyParameters();
	    	string meAsClient=generateClientMessage();
	    	//printf("Message Generated by me.... %s\n",meAsClient.c_str());
	    	processMessageRecievedFromClient(meAsClient);
	    }

	    //printf("Next Client : %s\n", clientIPs[next].c_str());

	    if (server == NULL) {
	        fprintf(stderr,"ERROR, no such host\n");
	        exit(0);
	    }
	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    bcopy((char *)server->h_addr, 
	         (char *)&serv_addr.sin_addr.s_addr,
	         server->h_length);
	    serv_addr.sin_port = htons(portno);
	    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
	        error("ERROR connecting");

	    bzero(buffer,1000);

	    //fgets(buffer,255,stdin);

	    //string msg="hello from server of gameObj"+convertIntToString(test) + "\n";
	    string msg=generatePrimaryServerMessage();

	    //printf("Message sending -> %s\n",msg.c_str() );
	    test++;

	    //printf("Sending msg to client %s\n",msg.c_str() );

	    n = write(sockfd,msg.c_str(),msg.length());
	    // jugad
	    while(n<0)
	    {
	    	printf("ERROR writing to socket\n");
	    	n = write(sockfd,msg.c_str(),msg.length());
	    }

	    if (n < 0) 
	         error("ERROR writing to socket");

	    if(msg.find("gameOver")==0)
	    {	    	
			State=3;
			if(msg.find("1")==9)
			{				
				if(mySelf->teamNumber == 1)
				{
					printf("YOU WON THE GAME\n");
					GameStatus=1;

				}
				else if(mySelf->teamNumber == 2)
				{
					printf("YOU LOST THE GAME\n");
					GameStatus=2;
				}
			}
			else if(msg.find("2")==9)
			{
				if(mySelf->teamNumber == 2)
				{
					printf("YOU WON THE GAME\n");
					GameStatus=1;
				}
				else if(mySelf->teamNumber == 1)
				{
					printf("YOU LOST THE GAME\n");
					GameStatus=2;
				}
			}
			printf("THANK YOU !!!!\n");
			//exit(0);
	    }

	    bzero(buffer,1000);
	    //printf("Message written...\n");
	    n = read(sockfd,buffer,1000);
	    if (n < 0) 
	         error("ERROR reading from socket");

	    string updateFromClient(buffer);

	    //printf("Message Recieved from client %s\n",buffer );

	    //printf("Message Recieved From Client : %s\n", buffer );
	    processMessageRecievedFromClient(updateFromClient);
	    //printf("Message Received : %s\n",buffer);
	    close(sockfd);
	}
	return 0;
}

int listenGameUpdates()
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[1000];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 4001;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
	      sizeof(serv_addr)) < 0) 
	      error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	int texturesSet=0;

	while(1)
	{
		if(mySelf!=NULL)
		{
			if(mySelf->dead==1)
			{
				if(deadTimer<=time(0))
				{
					mySelf->dead=0;
					deadTimer=0;
				}
			}
		}
		

	     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	     
	     if (newsockfd < 0) 
	          error("ERROR on accept");
	     bzero(buffer,1000);
	     n = read(newsockfd,buffer,1000);
	     if (n < 0) error("ERROR reading from socket");
	     //printf("Here is the message: %s\n",buffer);

	     string msgRcvd = string(buffer);

	     if(!texturesSet)
	     {
	     	printf("First msg from Server -> %s\n", buffer);
	     	printf("Before initialize game obj\n");
	     	intiailiseGameObj(msgRcvd);


  			if(mySelf->teamNumber == 1)
  			{
  				attack_on_castle=2;
  				opponent_player_1_IPAddress = gameObj.team2.player1.ipAddress;
  				opponent_player_2_IPAddress = gameObj.team2.player2.ipAddress;

  				// Setting up upper part as obstacle
  				for(int j=0;j<630;j++)
  				{
  					int y=900-j;
  					for(int i=1400; i>=((1085*900+ 630*315 -1085*y )/630) ;i--)
  					{
  						graph_team1[i][j]=1;
  					}
  				}

  			}
  			else if(mySelf->teamNumber == 2)
  			{
  				attack_on_castle=1;
  				opponent_player_1_IPAddress = gameObj.team1.player1.ipAddress;
  				opponent_player_2_IPAddress = gameObj.team1.player2.ipAddress;

  				// Setting up lower part as obstacle
  				for(int j=210;j<=900;j++)
  				{
  					int y=900-j;
  					for(int i=0; i<((690*1195 -1195*y )/690) ;i++)
  					{
  						graph_team2[i][j]=1;
  					}
  				}
  			}
  			printf("After initialize game obj\n");
	     	
	     	texturesSet=1;
	     }
	     else
	     {
	     	if(msgRcvd.find("gameOver")==0)
		    {	    	
				if(msgRcvd.find("1")==9)
				{				
					if(mySelf->teamNumber == 1)
					{
						printf("YOU WON THE GAME\n");
						GameStatus=1;
					}
					else if(mySelf->teamNumber == 2)
					{
						printf("YOU LOST THE GAME\n");
						GameStatus=2;
					}
				}
				else if(msgRcvd.find("2")==9)
				{
					if(mySelf->teamNumber == 2)
					{
						printf("YOU WON THE GAME\n");
						GameStatus=1;
					}
					else if(mySelf->teamNumber == 1)
					{
						printf("YOU LOST THE GAME\n");
						GameStatus=2;
					}
				}
				printf("THANK YOU !!!!\n");
				State=3;
		    }
	     	processServerMessage(msgRcvd);
	     }
	     string replyFromClient=generateClientMessage();
	     n = write(newsockfd,replyFromClient.c_str(),replyFromClient.length());
	     if (n < 0) error("ERROR writing to socket");
		 close(newsockfd);
	}
	close(sockfd);
    return 0; 
}

void listenGameUpdatesThread()
{
	pthread_t networkThread;

	cout << " Before Creation Thread ";

	int rc = pthread_create(&networkThread, NULL, listenGameUpdates,
			NULL);

	if (rc)
	{
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}
}

void sendJoinMessageToAllThread()
{
	if(mySelf->teamNumber == 1)
    {
        attack_on_castle=2;
        printf("Setting up my enemy....\n");
        opponent_player_1_IPAddress = gameObj.team2.player1.ipAddress;
        opponent_player_2_IPAddress = gameObj.team2.player2.ipAddress;

		// Setting up upper part as obstacle
		for(int j=0;j<630;j++)
		{
			int y=900-j;
			for(int i=1400; i>=((1085*900+ 630*315 -1085*y )/630) ;i--)
			{
				graph_team1[i][j]=1;
			}
		}

    }
    else if(mySelf->teamNumber == 2)
    {
        attack_on_castle=1;
        printf("Setting up my enemy....\n");
        opponent_player_1_IPAddress = gameObj.team1.player1.ipAddress;
        opponent_player_2_IPAddress = gameObj.team1.player2.ipAddress;

		// Setting up lower part as obstacle
		for(int j=210;j<=900;j++)
		{
			int y=900-j;
			for(int i=0; i<((690*1195 -1195*y )/690) ;i++)
			{
				graph_team2[i][j]=1;
			}
		}
    }

    printf("My enemies: %s and %s \n", opponent_player_1_IPAddress.c_str() , opponent_player_2_IPAddress.c_str());

	pthread_t networkThread;

	cout << " Before Creation Thread ";

	int rc = pthread_create(&networkThread, NULL, sendJoinMessageToAll,
			NULL);

	if (rc)
	{
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}
}
