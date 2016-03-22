#ifndef HERO_H_
#define HERO_H_

#include <iostream>

class Hero
{
	public: 
		int heroID;
		int texture[8];
		int xPosition;
		int yPosition;
		int health;
		int powerID;
		int attackSpeed;
		int attackSpeedTimer;
		int movementSpeed;
		int movementSpeedTimer;
		int powerCount;
		int playerNumber;
		int teamNumber;
		int stunMySelf;
		std::string ipAddress;
		int dead;

	Hero()
	{
		ipAddress="";
		health=100;
		health=100;
		dead=0;
	}
};

#endif