//gooseExcapeActors.hpp
#ifndef GOOSE_ESCAPE_ACTORS
#define GOOSE_ESCAPE_ACTORS
#include <cmath>
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"

class Actor
{
  private:
    int actorChar;      
    int location_x, location_y;
	string buffState; //buff state for the character
	
  public:
    Actor()
    {
        actorChar = int('A');
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        put_actor();
    }
	
    Actor(char initPlayerChar, int x0, int y0, string intialState)
    {
        change_char(initPlayerChar);
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        update_location(x0,y0);
        buffState = intialState; // set buff state to the inital value
    }
    
    //gets x and y values
    int get_x() const
    {
        return location_x;
    }
    
    int get_y() const
    {
        return location_y;
    }
    
    //gives the location of the Actor
    string get_location_string() const
    {
        char buffer[80];
        itoa(location_x,buffer,10);
        string formatted_location = "(" + string(buffer) + ",";
        itoa(location_y,buffer,10);
        formatted_location += string(buffer) + ")";
        return formatted_location;
    }
    
    //change the Actor's character
    void change_char(char new_actor_char)
    {
        actorChar = min(int('~'),max(int(new_actor_char),int(' ')));
    }
	
	//return if the actor can move or not
    bool can_move(int delta_x, int delta_y) const
    {
        int new_x = location_x + delta_x;
        int new_y = location_y + delta_y;

        return new_x >= MIN_BOARD_X && new_x <= MAX_BOARD_X
          && new_y >= MIN_BOARD_Y && new_y <= MAX_BOARD_Y;
    }
	
	//updates the location of the Actor
    void update_location(int delta_x, int delta_y)
    {
        if (can_move(delta_x, delta_y))
        {
            terminal_clear_area(location_x, location_y, 1, 1);
            location_x += delta_x;
            location_y += delta_y;
            put_actor();
        }
    }
    
    //placement of Actor
    void put_actor() const
    {
        terminal_put(location_x, location_y, actorChar);
        terminal_refresh();
    }
    
    /////new functions/////
    
    //updates the buff
    void update_Buff(string buff)
    {
    	buffState = buff;
	}
	
	//returns the buff
    string get_Buff() const
    {
		return buffState;
    }
    
    //changes the location on the board
    void change_location(int x, int y)
    {
        terminal_clear_area(location_x, location_y, 1, 1);
		location_x = x;
        location_y = y;
        put_actor();
    }
};
#endif
