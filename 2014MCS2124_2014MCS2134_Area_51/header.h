#ifndef HEADER_H
#define HEADER_H

#include <queue> 
#include <vector>
#include <iostream>
#include <cstdlib>
#include <set>
#include <cmath>
#include <stack>
#include <string>
#include <cmath> 
#include <time.h>
#include <sys/time.h>
using namespace std;

#include <png.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>

#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <X11/Xlib.h>

#include "Utility.h"

//Screen Constants
const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 900;
const int SCREEN_FPS = 40;

#define MOVEMENT_SPEED_FAST 50
#define MOVEMENT_SPEED_MEDIUM 15
#define MOVEMENT_SPEED_SLOW 1

#define ATTACK_SPEED_FAST 5
#define ATTACK_SPEED_MEDIUM 2
#define ATTACK_SPEED_SLOW 1

int noOfClients=0;
int State=0;
int server;
std::string server_ip_address;
int server_port=4000;


int x_Coord=-1;
int y_Coord=-1;

int	attack_on_castle;
int 	attack_count_castle;
int	item_used=-1;
int 	magical_power_used;
int 	item_taken=-1;

string myIPAddress="";

#include "loadpng.h"
#include "loadpng.cpp"
#include "SetTexture.cpp"
#include "multiset.cpp"
#include "Hero.cpp"
#include "Team.cpp"
#include "Game.cpp"


Hero *mySelf=NULL;

Game gameObj;
int Server=-1;
int graph_team1[SCREEN_WIDTH+1][SCREEN_HEIGHT+1];
int graph_team2[SCREEN_WIDTH+1][SCREEN_HEIGHT+1];
string heros_path ="Heros/hero_";
string items_path ="Items/";

string opponent_player_1_IPAddress;
int opponent_player_1_attacks;

string opponent_player_2_IPAddress;
int opponent_player_2_attacks;

/*
myPowerID =0; // Disabler
myPowerID =1; // Slower
myPowerID =2; // Burst Damage
myPowerID =3; // Stunner
*/
int myPowerID=-1;
int iUsedMyMagicalPower=-1;

int disableTeam=-1;
int disableResumeTimer=-1;
#define DISAMBLER_TIME 10

int stun_team1_player1;
int stun_team1_player2;
int stun_team2_player1;
int stun_team2_player2;

int x_Mouse;
int y_Mouse;
int GameStatus=0;

int attack_x;
int attack_y;
int display_attack;
#define ATTACK_DISPLAY_TIMER 5

int stun_team1_resume_timer;
int stun_team2_resume_timer;
#define STUN_TIME 10

int deadTimer;

#define POWER_COUNT 10

#include "ItemHandlerAndSpawnChecker.cpp"

#include "networkThread.cpp"
#include "LUtil.cpp"
#include "BotPlayers.cpp"

/*texture[26] = hide.bmp
texture[27] = white.png
texture[28] = green.png
texture[29] = red.png*/

#endif
