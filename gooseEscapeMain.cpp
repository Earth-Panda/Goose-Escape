//gooseEscapeMain.cpp
/*
	Jason Heng
	Selvin Wen
*/

#include <BearLibTerminal.h>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"
#include <ctime>
//set up the console. 
Console out;

int main()
{
	//Set up the window. 
    terminal_open();
  	terminal_set(SETUP_MESSAGE);
 	
 	//intialize the random number
 	srand(time(NULL));
 	
 	//set the game to not close
 	bool gameClose = false;
 	
 	//this allows for the game to restart if the player has not closed the game
 	while(!gameClose)
 	{
	    // Declare the array that will hold the game board "map"
	    int map[NUM_BOARD_X][NUM_BOARD_Y] = {EMPTY};
		
		//values for level select
		int level_num = 0;
		level_num = random(LEVEL_NUM);
		
		//read in values
		ifstream fin_L1("level_1.txt");
		ifstream fin_L2("level_2.txt");
		ifstream fin_L3("level_3.txt");
		ifstream fin_L4("level_4.txt");
		
		//opened correctly
		if(!fin_L1 || !fin_L2 || !fin_L3 || !fin_L4)
		{
			cout << "error in reading file." << endl;
			return 1;
		}
	
		//build map depeding on file randomly selcted
		switch (level_num) 
		{
			case MAP_1:
				makeLevel(fin_L1, map);
				break;
			case MAP_2:
				makeLevel(fin_L2, map);
				break;
			case MAP_3:
				makeLevel(fin_L3, map);
				break;
			case MAP_4:
				makeLevel(fin_L4, map);
				break;
		}

		//while loop done boolean
		bool done = false;

		//making the actors, will spawn randomly inside the boundry
		//make the player
		Actor player(PLAYER_CHAR, random(NUM_BOARD_X),random(NUM_BOARD_Y), "None"); 
		
		//make the goose (monster)
		Actor monster(MONSTER_CHAR, random(NUM_BOARD_X),random(NUM_BOARD_Y), "None");
		
		//make the powerup
		Actor powerUP(POWER_CHAR, random(NUM_BOARD_X),random(NUM_BOARD_Y), "None");
		
		//checks spawn conditions, actors don't overlap, spawn is not in a wall
		while((captured(player,monster) || captured(player,powerUP)) || !done) 
		{
			if(tileCheck(player,map,SHALL_NOT_PASS))
			{
				player.change_location(random(NUM_BOARD_X),random(NUM_BOARD_Y));
			}
			
			else if(tileCheck(monster,map,SHALL_NOT_PASS))
			{
				monster.change_location(random(NUM_BOARD_X),random(NUM_BOARD_Y));
			}
			
			else if(tileCheck(powerUP,map,SHALL_NOT_PASS))
			{
				powerUP.change_location(random(NUM_BOARD_X),random(NUM_BOARD_Y));
			}
			
			else
			{
				done = true;
			}
		}
		
		// Call the function to print the game board
	  	boardPrint(map);
			  		
		// Printing the instructions
	    out.writeLine("New game Has begun. Escape the Goose!" + 
					  monster.get_location_string() + " PowerUP " 
			     	  + powerUP.get_location_string());
		out.writeLine("Use the arrow keys to move."
					  "If the goose catches you, you lose!");
		out.writeLine("PowerUPs! will spawn to help you out! Good luck!");
	
	 	// intital values
	    int keyEntered = TK_A; 
	    bool pow_Grab = false;
	    int turnCount = 0;
	    int turnMax = 0;
	    int buffID = 0;
	    bool frozen = false;
	    bool speedBoost = false;
	    bool won = false;
	    int gooseMoveTurn = 0;
	    /*
		    This is the main game loop.  It continues to let the player 
			give input as long as they do not press escape or close, 
			they are not captured by the goose(and isn't invicible), 
			and they haven't won yet.
		*/
		
	    while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE 
	          && !(captured(player,monster) 
			  && player.get_Buff() != "Invincible") && !won)
		{
			//generate random ID for buff (1-4)
			buffID = random(BUFFNUM);	

		    // get player key press
		    keyEntered = terminal_read();
			
			//game not closed
	        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
	        {
	            // move the player by their movement
	    	    movePlayer(keyEntered,player,map);
				
				//make sure goose is not frozen and speedUp is not active
                if(!frozen && !speedBoost 
				   && gooseMoveTurn % GOOSEMOVEFACTOR != 0)
                {
                    // call the goose's chase function
                    moveGoose(monster, player, map);
                }
				
				//checks to see if speedUp powerup is active
				if(speedBoost == true)
				{
					//moves goose every other turn while powerup is active
					if (turnCount % SPEEDBUFFVALUE == 0)
					{
						moveGoose(monster, player, map);
					}
				}
				
				//check if a powerup has not been picked up
				if(!pow_Grab)
				{
					//set value to close if statment after compleation
					pow_Grab = captured(player,powerUP);
					
					//check if they are currently overlapping
					if(captured(player,powerUP))
					{
						//update player's buff
						player.update_Buff(buffUP(buffID));
						
						//print said buff to the console
						out.writeLine("Buff: " + player.get_Buff());
						
						//calls apropriate fucntion according to the buff ID
						switch (buffID) 
						{
							case INVINCIBLEID:
								invincible_UP(turnCount, turnMax);
								break;
								
							case TPID:
								teleport(monster, random(NUM_BOARD_X),
										 random(NUM_BOARD_Y));
								//no turns for this powerup so reset imidately
								turnMax = 0;
								break;
								
							case SPEEDID:
								speedUP(turnCount, turnMax, speedBoost);
								break;
								
							case TIMESTOPID:
								time_Stop(turnCount, turnMax, frozen);
								break;
						}
					}	
				}
				
				/*
				increase turn count when the turnCount is less 
				than the buff duration
				*/
				else if(pow_Grab && turnCount < turnMax)
				{
					turnCount++;
				}
				
				//ends any buff and resets values if the turns are up
				else if(turnCount >= turnMax)
				{
					//change the powerup location
					do 
					{
						teleport(powerUP, random(NUM_BOARD_X),
								 random(NUM_BOARD_Y));
					}while(tileCheck(powerUP, map, SHALL_NOT_PASS));
					
					turnCount = 0;
					pow_Grab = false;
					player.update_Buff("None");
					out.writeLine("Buff: " + player.get_Buff());
					frozen = false;
					speedBoost = false;
				} 
				
				//makes sure win conditions are met
				if((captured(player,monster) 
				  && player.get_Buff() == "Invincible") 
				  || tileCheck(player, map, WINNER))
				{
					won = true;
				}
				else
				{
					won = false;
				}
				
				// Call the function to print the game board
	  			boardPrint(map);
	  			
				//refresh terminal
				terminal_refresh();
				gooseMoveTurn++;	 
	        }
	  	}
		
		//check game window no closed
	    if (keyEntered != TK_CLOSE)
	    {
	        /* 
				output why: did the goose get us, or did we win 
				(catching it while invicible or reaching the point)?
			*/
			if(won)
			{
				out.writeLine("You've won!"); 
			}

			else
			{
				out.writeLine("The Goose has caught you.");
			}
	    }
		
		//if the game window is closed exit the while loop
	    if (keyEntered == TK_CLOSE)
	    {
	    	gameClose = true;
		}
		
		//clear the screen for a new game
		terminal_clear();
	}
	
	//game is done, close it  
    terminal_close();
}
