//gooseExcapeGamePlay.cpp
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

extern Console out;

// print the game board function
void boardPrint(int map[NUM_BOARD_X][NUM_BOARD_Y])
{	
	//print out the display at each index
	for(int co_X = 0; co_X < NUM_BOARD_X; co_X++)
	{
		for(int co_Y = 0; co_Y < NUM_BOARD_Y; co_Y++)
		{
			/*
			if a character is at a ceratin place, 
			output the corrisponding character
			*/
			if(map[co_X][co_Y] == SHALL_NOT_PASS)
			{
				terminal_put(co_X,co_Y,WALL_CHAR);
			}
			if(map[co_X][co_Y] == WINNER)
			{
				terminal_put(co_X,co_Y,WIN_CHAR);
			}
		}
	}
}

//Do something when the goose captures the player
bool captured(Actor const & player, Actor const & monster)
{
	return (player.get_x() == monster.get_x() 
     		&& player.get_y() == monster.get_y());
}

//Move the player to a new location based on the user input. 
void movePlayer(int key, Actor & player, int map[NUM_BOARD_X][NUM_BOARD_Y])
{
    //move charater dependin on input
    int yMove = 0, xMove = 0;
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
    //check if the player can move tin that direction
    if (player.can_move(xMove, yMove) 
      && map[player.get_x() + xMove][player.get_y() + yMove] != SHALL_NOT_PASS)
        player.update_location(xMove, yMove);
}

//move the goose according to player movement
void moveGoose(Actor & monster, Actor const & player,
			   int map[NUM_BOARD_X][NUM_BOARD_Y])
{
    int yMove = 0, xMove = 0;
    int yFromPlayer = 0, xFromPlayer = 0;
    //get y distance to player
    yFromPlayer = player.get_y() - monster.get_y();
    //get x distance to player
    xFromPlayer = player.get_x() - monster.get_x();
    
    if (abs(yFromPlayer) >= abs(xFromPlayer) && yFromPlayer != 0)
    {
		//move the goose in the y direction
        yMove = yFromPlayer/abs(yFromPlayer);    
    }
    if (abs(xFromPlayer) >= abs(yFromPlayer) && xFromPlayer != 0)
    {
		//move the goose in the x direction
        xMove = xFromPlayer/abs(xFromPlayer);
    }
    
	//move the goose accordingly 
   	monster.update_location(xMove, yMove);
}

//this determines if the player has overlaped with a tile
bool tileCheck(Actor const & player, int map[NUM_BOARD_X][NUM_BOARD_Y],
			   int const TILE)
{
	//return if the player tile overlaped with a tile
	return (map[player.get_x()][player.get_y()] == TILE);
}

//////////new functions added below//////////

//random number generator
int random(int const NUM)
{
	int randomNumber = 0;
 	randomNumber = rand() % NUM + 1;
 	return randomNumber;
}

//returns the buff type in string format
string buffUP(int id)
{
	string buffType = " ";
	
	switch (id) 
	{
		case INVINCIBLEID:
			buffType = "Invincible";
			break;
		case TPID:
			buffType = "Teleport";
			break;
		case SPEEDID:
			buffType = "Speed";
			break;
		case TIMESTOPID:
			buffType = "ZA WARUDO";
			break;
	}
	return buffType;
}

//will change values to make the character fast
void speedUP(int & turncount, int & turnMax, bool & speedBoost)
{
	turncount = 0;
	turnMax = SPEEDTURN;
	speedBoost = true;
}

//will change values to make the character invincible
void invincible_UP(int & turncount, int & turnMax)
{
	turncount = 0;
	turnMax = INVINCIBLETURN;
}

//will teleport an actor to a rondom location
void teleport(Actor & actor, int new_x, int new_y)
{
	actor.change_location(new_x, new_y);
}

//sets values to stop time
void time_Stop(int & turncount, int & turnMax, bool & frozen)
{
	turncount = 0;
	turnMax = TIMETURN;
	frozen = true;
}

//a loop that fills an array by reading from a file
void makeLevel(ifstream & fin, int map[NUM_BOARD_X][NUM_BOARD_Y])
{
	char character = '0';
	int space = 0;
			
	for(int co_Y = 0; co_Y < NUM_BOARD_Y; co_Y++)
    {
        for(int co_X = 0; co_X < NUM_BOARD_X; co_X++)
        {
            fin >> character;
            
			if(int(character) == UNICODE_0)
            {
            	space = EMPTY;
			}
			
            else if(int(character) == UNICODE_1)
            {
            	space = SHALL_NOT_PASS;
			}
			
			else if(int(character) == UNICODE_2)
            {
            	space = WINNER;
			}
			
            map[co_X][co_Y] = space;
        }
    }
}
