//gooseExcapeGamePlay.cpp
#ifndef GOOSE_ESCAPE_GAMEPLAY
#define GOOSE_ESCAPE_GAMEPLAY
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"

//our global constants for creating gameboard 
const int EMPTY = 0;
const int SHALL_NOT_PASS = 1;
const int WINNER = 2;
const int UNICODE_1 = 49;
const int UNICODE_0 = 48;
const int UNICODE_2 = 50;
const int BUFFNUM = 4;
const int SPEEDBUFFVALUE = 2;
const int GOOSEMOVEFACTOR = 4;
const int SPEEDTURN = 10;
const int INVINCIBLETURN = 15;
const int TIMETURN = 10;
const int LEVEL_NUM = 4;

//buff ids
const int INVINCIBLEID = 1;
const int TPID = 2;
const int SPEEDID = 3;
const int TIMESTOPID = 4;

//map ids
const int MAP_1 = 1;
const int MAP_2 = 2;
const int MAP_3 = 3;
const int MAP_4 = 4;

//display characters
const int PLAYER_CHAR = int('@');
const int MONSTER_CHAR = int('G');
const int WALL_CHAR = int('o');
const int WIN_CHAR = int('%'); 
const int POWER_CHAR = int('$');
//% sign, a special character used in the ancient game "Rogue"

// print the game board function protype
void boardPrint(int map[NUM_BOARD_X][NUM_BOARD_Y]);

//Do something when the goose captures the player
bool captured(Actor const & player, Actor const & monster);

//Move the player to a new location based on the user input.
void movePlayer(int key, Actor & player, int map[NUM_BOARD_X][NUM_BOARD_Y]);

//move the goose according to player movement
void moveGoose(Actor & monster, Actor const & player, 
			   int map[NUM_BOARD_X][NUM_BOARD_Y]);

//determines the winner
bool tileCheck(Actor const & player, int map[NUM_BOARD_X][NUM_BOARD_Y],
			   const int TILE);

//////////new functions added below//////////

//random number generator
int random(int const NUM);

//returns the buff type in string format
string buffUP(int id);

//will change values to make the character fast
void speedUP(int & turncount, int & turnMax, bool & speedBoost);

//will change values to make the character invincible
void invincible_UP(int & turncount, int & turnMax);

//will teleport an actor to a rondom location
void teleport(Actor & actor, int new_x, int new_y);

//sets values to stop time
void time_Stop(int & turncount, int & turnMax, bool & frozen);

//a loop that fills an array by reading from a file
void makeLevel(ifstream & fin, int map[NUM_BOARD_X][NUM_BOARD_Y]);
#endif
