/*
 * USER_INTERFACE.c
 *
 * Created: 17.09.2015 15:27:58
 *  Author: michalma
 */ 

#include "USER_INTERFACE.h"
#include "PING_PONG_LIB.h"
#include "COM_LIB.h"
#include "MCP_ADDRESSES.h"
#include <avr/eeprom.h>
#include <stdio.h>

/****************************************************************************
* \brief Initialize the user interface
* 
* TODO EEPROM highscores
*
****************************************************************************/
void interface_init()
{
	high_score interface_high_scores;
	volatile char *ext_ram = (char *) SRAM_USERNAME_ADDR;
	volatile char *high_scores_sram = (char *) EEPROM_HIGH_SCORES_BASE_ADDR;
	uint8_t i, j;
	
	for(i = 0; i < USERNAME_LENGTH; i++)
	{
		ext_ram[i] = 'A';
	}
	/*
	interface_high_scores = high_score_read(EEPROM_HIGH_SCORES_BASE_ADDR);
	
	for(i = 0; i < HIGH_SCORES_TABLE_LENGTH; i++)
	{
		for (j = 0; j < USERNAME_LENGTH; i++)
		{
			high_scores_sram[SRAM_HIGH_SCORES_ADDR + i * 9 + j] = interface_high_scores.username[i][j];	
		}
		high_scores_sram[SRAM_HIGH_SCORES_ADDR + i * 9 + 5] = ' ';	
		high_scores_sram[SRAM_HIGH_SCORES_ADDR + i * 9 + 6] = (interface_high_scores.score[i] / 100) + 16;
		high_scores_sram[SRAM_HIGH_SCORES_ADDR + i * 9 + 7] = ((interface_high_scores.score[i] % 100) / 10) + 16;
		high_scores_sram[SRAM_HIGH_SCORES_ADDR + i * 9 + 8] = ((interface_high_scores.score[i] % 100) % 10) + 16;
	}*/	
}

/****************************************************************************
* \brief The main function serving the game
*
* \param in joystick  data
****************************************************************************/
void interface_state_machine()
{	
	static InterfaceState state = State_Username;

	clear_oled();
	
	switch(state)
	{
		case State_Username :
			state = interface_username();
			break;
		
		case State_NewGame :
			state = interface_new_game();
			break;
		
		case State_Tutorial :
			send_game_mode(Tutorial);
			state = interface_tutorial();
			break;
		
		case State_Easy :
			send_game_mode(Easy);
			state = interface_print_mode(Easy);
			break;
		
		case State_Normal :
			send_game_mode(Normal);
			state = interface_print_mode(Normal);
			break;
		
		case State_Hard :
			send_game_mode(Hard);
			state = interface_print_mode(Hard);
			break;
		
		case State_Insane :
			send_game_mode(Insane);
			state = interface_print_mode(Insane);
			break;
		
		case State_HighScores :
			state = interface_high_scores();
			break;
		
		case State_Options :
			state = interface_options();
			break;
		
		case State_Sound :
			state = interface_sound();
			break;
		
		case  State_Mode :
			state = interface_mode();
			break;
		
		case State_Playing:

			state = interface_playing();
			break;
		
		case State_Endgame:
		//Ending animation, probably return to highscore
		//after some time
			state = State_HighScores;
			break;
	}
}

/****************************************************************************
* \brief Function call during the set username phase
*

****************************************************************************/
InterfaceState interface_username()
{
	volatile char *ext_ram = (char *) SRAM_USERNAME_ADDR;
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y, change_x;
	ChangeTouchpadData change_touch_data;
	TouchpadData touch_data;
	char name[USERNAME_LENGTH];
	char index = 0;
	
	for(int i=0; i<USERNAME_LENGTH;i++)
	{
		name[i] = ext_ram[i];
	}
	
	position = read_joystick_position();
	direction = read_joystick_direction(position);
	change_y = change_yaxis(direction);
	change_x = change_xaxis(direction);
	
	set_position(12, 0);
	print_string("Username");
	revert_colour_line(0);
	
	set_position(4,4);
	
	while (1)
	{
		position = read_joystick_position();
		direction = read_joystick_direction(position);
		change_y = change_yaxis(direction);
		change_x = change_xaxis(direction);
		
		if (change_x == Right)
		{
			index = (index + 1) % 5;
		}
		else if (change_x == Left)
		{
			index = index == 0 ? 4 : (index - 1);
		}
		
		if (change_y == Up)
		{
			name[index] = (name[index] == 'Z') ? 'A' : (name[index] + 1);
		}
		else if (change_y == Down)
		{
			name[index] = (name[index] == 'A') ? 'Z' : (name[index] - 1);
		}
		
		set_position(10, 4);
		for (int i = 0; i < 5; i++)
		{
			if (i == index)
			{
				print_inverted_char(name[i]);
			}
			else
			{
				print_char(name[i]);
			}
			print_char(' ');
		}
		
		refresh_oled();
		
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			for(int i=0; i<USERNAME_LENGTH;i++)
			{
				ext_ram[i] = name[i];
			}
			
			return State_NewGame;
		}
	}	
}

/****************************************************************************
* \brief Function call during the set new game phase
*

****************************************************************************/
InterfaceState interface_new_game()
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y, change_x;
	ChangeTouchpadData change_touch_data;
	TouchpadData touch_data;
	char scr_position = 0;
	
	position = read_joystick_position();
	direction = read_joystick_direction(position);
	change_y = change_yaxis(direction);
	change_x = change_xaxis(direction);
	
	set_position(13, 0);
	print_string("New game");
	revert_colour_line(0);
	set_position(3, 2);
	print_string("Tutorial");
	set_position(3, 3);
	print_string("Easy");
	set_position(3, 4);
	print_string("Normal");
	set_position(3, 5);
	print_string("Hard");
	set_position(3, 6);
	print_string("Insane");
	
	while(1)
	{
		position = read_joystick_position();
		direction = read_joystick_direction(position);
		change_y = change_yaxis(direction);
		change_x = change_xaxis(direction);
		
		if (change_x == Right)
		{
			return State_Options;
		}
		else if (change_x == Left)
		{
			return State_HighScores;
		}
		
		if (change_y == Up)
		{
			set_position(1, (scr_position % 5) + 2);
			print_char(' ');
			scr_position--;
		}
		else if (change_y == Down)
		{
			set_position(1, (scr_position % 5) + 2);
			print_char(' ');
			scr_position++;
		}
		
		set_position(1, (scr_position % 5) + 2);
		print_char('#');
		
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			switch(scr_position)
			{
				case 0:
					send_game_mode(Tutorial);
					return State_Tutorial;
				case 1:
					send_game_mode(Easy);
					return State_Easy;
				case 2:
					send_game_mode(Normal);
					return State_Normal;
				case 3:
					send_game_mode(Hard);
					return State_Hard;
				case 4:
					send_game_mode(Insane);
					return State_Insane;
			}
		}
		refresh_oled();
	}
}

/****************************************************************************
* \brief Function call during the set show high scores phase
*
* TODO EEPROM reading the high scores
*

****************************************************************************/
InterfaceState interface_high_scores()
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_x;
	char scr_position = 0;
	volatile char *high_scores = (char *) SRAM_HIGH_SCORES_ADDR;
	
	position = read_joystick_position();
	direction = read_joystick_direction(position);
	change_x = change_xaxis(direction);
	
	set_position(9,0);
	print_string("High Scores");
	revert_colour_line(0);
	
	/*
	for(int j = 0; j < HIGH_SCORES_TABLE_LENGTH; j + 9)
	{
		set_position(1, (scr_position % 5));
		for (int k = 0; k < 9; k++)
		{
			print_char(high_scores[j + k]);
		}
	}*/
	
	refresh_oled();
	
	while(1)
	{
		position = read_joystick_position();
		direction = read_joystick_direction(position);
		change_x = change_xaxis(direction);
		
		if (change_x == Right)
		{
			return State_NewGame;
		}
		else if (change_x == Left)
		{
			return State_Options;
		}
	}
		
}

/****************************************************************************
* \brief Function call during the options phase
*

****************************************************************************/
InterfaceState interface_options( )
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y, change_x;
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	char scr_position = 0;
	
	position = read_joystick_position();
	direction = read_joystick_direction(position);
	change_y = change_yaxis(direction);
	change_x = change_xaxis(direction);

	set_position(12,0);
	print_string("Options");
	revert_colour_line(0);
	set_position(3,3);
	print_string("Mode");
	set_position(3,4);
	print_string("Sound");
	set_position(3,5);
	print_string("Change Username");
	
	while(1)
	{
		position = read_joystick_position();
		direction = read_joystick_direction(position);
		change_y = change_yaxis(direction);
		change_x = change_xaxis(direction);
		
		if (change_x == Right)
		{
			return State_HighScores;
		}
		else if (change_x == Left)
		{
			return State_NewGame;
		}
		
		if (change_y == Up)
		{
			set_position(1, (scr_position % 3) + 3);
			print_char(' ');
			scr_position--;
		}
		else if (change_y == Down)
		{
			set_position(1, (scr_position % 3) + 3);
			print_char(' ');
			scr_position++;
		}
		
		set_position(1, (scr_position % 3) + 3);
		print_char('#');
		
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			switch(scr_position)
			{
				case 0:
					return State_Mode;
				case 1:
					return State_Sound;
				case 2:
					return State_Username;
			}
		}
		refresh_oled();
	}
}

/****************************************************************************
* \brief Function call during the set mode phase
*

****************************************************************************/
InterfaceState interface_mode()
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y;
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	char scr_position = 0;
	
	position = read_joystick_position();
	direction = read_joystick_direction(position);
	change_y = change_yaxis(direction);

	set_position(14,0);
	print_string("Mode");
	revert_colour_line(0);
	set_position(3,3);
	print_string("Reverse");
	set_position(3,5);
	print_string("Normal");
	
	
	set_position(1, (scr_position % 2) * 2 + 3);
	print_char('#');
	
	while(1)
	{
		position = read_joystick_position();
		direction = read_joystick_direction(position);
		change_y = change_yaxis(direction);
		
		if (change_y == Up)
		{
			set_position(1, (scr_position % 2) * 2 + 3);
			print_char(' ');
			scr_position--;
		}
		else if (change_y == Down)
		{
			set_position(1, (scr_position % 2) * 2 + 3);
			print_char(' ');
			scr_position++;
		}
		
		set_position(1, (scr_position % 2) * 2 + 3);
		print_char('#');
		
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			switch(scr_position)
			{
				case 0:
					send_game_mode(Normal_settings);
					printf("Normal\n");
					break;
				case 1:
					//send_game_mode(Reverse_settings);
					//printf("Reverse\n");
					break;
			}
			return State_Options;
		}
		refresh_oled();
	}
}

/****************************************************************************
* \brief Function call during the set sound phase
*

****************************************************************************/
InterfaceState interface_sound()
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y;
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	char scr_position = 0;
	
	position = read_joystick_position();
	direction = read_joystick_direction(position);
	change_y = change_yaxis(direction);

	set_position(14,0);
	print_string("Sound");
	revert_colour_line(0);
	set_position(3,3);
	print_string("On");
	set_position(3,5);
	print_string("Off");
	
	while(1)
	{
		position = read_joystick_position();
		direction = read_joystick_direction(position);
		change_y = change_yaxis(direction);
		
		if (change_y == Up)
		{
			set_position(1, (scr_position % 2) * 2 + 3);
			print_char(' ');
			scr_position--;
		}
		else if (change_y == Down)
		{
			set_position(1, (scr_position % 2) * 2 + 3);
			print_char(' ');
			scr_position++;
		}
		
		set_position(1, (scr_position % 2) * 2 + 3);
		print_char('#');
		
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			switch(scr_position)
			{
				case 0:
				printf("ON\n");
				break;
				case 1:
				printf("OFF\n");
				break;
			}
			return State_Options;
		}
		refresh_oled();
	}
}

/****************************************************************************
* \brief Function call during the set sound phase
*
****************************************************************************/
InterfaceState interface_tutorial()
{
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	JoystickPosition position;
	set_position(14,0);
	print_string("Tutorial");
	revert_colour_line(0);
	set_position(0,6);
	print_string("Press LEFT BUTTON to continue...");
	refresh_oled();
	
	set_position(0,1);
	print_string("To move servo move joystick");
	refresh_oled();
	while(! change_touch_data.leftButton)
	{
		position = read_joystick_position();
		touch_data = read_touchpad_data();
		touch_data.rightTouchPad = 127;
		touch_data.rightButton = 0;
		send_joystick_possition(position, touch_data);
		change_touch_data = change_touchpad_data(touch_data);
	}
	change_touch_data.leftButton = 0;
	
	set_position(0,1);
	print_string("To move motor touch RIGHT SLIDER");
	refresh_oled();
	
	while(! change_touch_data.leftButton)
	{
		touch_data = read_touchpad_data();
		touch_data.rightButton = 0;
		send_joystick_possition(position, touch_data);
		change_touch_data = change_touchpad_data(touch_data);
	}
	change_touch_data.leftButton = 0;
	
	set_position(0,1);
	print_string("  To shoot touch RIGHT BUTTON   ");
	refresh_oled();
	
	while(! change_touch_data.leftButton)
	{
		touch_data = read_touchpad_data();
		touch_data.rightTouchPad = 127;
		send_joystick_possition(position, touch_data);
		change_touch_data = change_touchpad_data(touch_data);
	}
	change_touch_data.leftButton = 0;
	
	set_position(0,1);
	print_string("    Try everything together     ");
	refresh_oled();
	
	while(! change_touch_data.leftButton)
	{
		touch_data = read_touchpad_data();
		position = read_joystick_position();
		send_joystick_possition(position, touch_data);
		change_touch_data = change_touchpad_data(touch_data);
	}
	change_touch_data.leftButton = 0;
	change_touch_data.rightButton = 0;
	
	return State_NewGame;
}


/****************************************************************************
* \brief Print out game mode on OLED
*
* \param in game mode
****************************************************************************/
InterfaceState interface_print_mode(GameModes mode)
{
	switch (mode)
	{
		case Easy :
			set_position(14,0);
			print_string("Easy");
			break;
			
		case Normal :
			set_position(12,0);
			print_string("Normal");
			break;
			
		case Hard :
			set_position(12,0);
			print_string("Hard");
			break;
						
		case Insane :
			set_position(12,0);
			print_string("Insane");
			break;
	}
	revert_colour_line(0);
	refresh_oled();
	
	return State_Playing;
	
}

InterfaceState interface_playing()
{
	CANMessage canMessageNode2;
	TouchpadData data;
	JoystickPosition position;
	
	do
	{
		position = read_joystick_position();
		data = read_touchpad_data();
		
		send_joystick_possition(position, data);
		_delay_ms(1);
		
		if(CAN_receive_message(&canMessageNode2) != SUCCESS)
		{
			canMessageNode2.ID = 0xFF;	
		}
		
	} while(canMessageNode2.ID != 0x05);
	
	return State_Endgame; 
}