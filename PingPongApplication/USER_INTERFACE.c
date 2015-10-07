/*
 * USER_INTERFACE.c
 *
 * Created: 17.09.2015 15:27:58
 *  Author: michalma
 */ 

#include "USER_INTERFACE.h"
#include "PING_PONG_LIB.h"
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
	volatile char *ext_ram = (char *) SRAM_USERNAME_ADDR;
	volatile char *eeprom = (char *) EEPROM_HIGH_SCORES_BASE_ADDR;

	for(int i = 0; i < SRAM_USERNAME_ADDR_LENGTH; i++)
	{
		ext_ram[i] = 'A';
	}
	
	/*for(int j = 0; j < EEPROM_HIGH_SCORES_ADDR_LEN; j++)
	{
		for (int k = 0; k < 5; k++)
		{
			eeprom_busy_wait();
			eeprom_write_byte(eeprom[k], (uint8_t) 'A'+k);
		}
		eeprom_busy_wait();
		eeprom_write_dword(eeprom[j + 5],(uint32_t) 0);
	}*/
}

/****************************************************************************
* \brief The main function serving the game
*
* \param in joystick calibration data
****************************************************************************/
void interface_state_machine(JoystickPosition calibration)
{
	static InterfaceState state = State_Username;
	
	clear_oled();
	
	switch(state)
	{
		case State_Username :
		state = interface_username(calibration);
		break;
		
		case State_NewGame :
		state = interface_new_game(calibration);
		break;
		
		case State_Tutorial :
		state = interface_tutorial(calibration);
		break;
		
		case State_Easy :
		state = interface_easy(calibration);
		break;
		
		case State_Normal :
		state = interface_normal(calibration);
		break;
		
		case State_Hard :
		state = interface_hard(calibration);
		break;
		
		case State_Insane :
		state = interface_insane(calibration);
		break;
		
		case State_HighScores :
		state = interface_high_scores(calibration);
		break;
		
		case State_Options :
		state = interface_options(calibration);
		break;
		
		case State_Sound :
		state = interface_sound(calibration);
		break;
		
		case  State_Mode :
		state = interface_mode(calibration);
		break;
	}
}

/****************************************************************************
* \brief Function call during the set username phase
*
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_username(JoystickPosition calibration)
{
	volatile char *ext_ram = (char *) SRAM_USERNAME_ADDR;
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y, change_x;
	ChangeTouchpadData change_touch_data;
	TouchpadData touch_data;
	char name[SRAM_USERNAME_ADDR_LENGTH];
	char index = 0;
	
	for(int i=0; i<SRAM_USERNAME_ADDR_LENGTH;i++)
	{
		name[i] = ext_ram[i];
	}
	
	position = read_joystick_position(calibration);
	direction = read_joystick_direction(position);
	change_y = change_yaxis(direction);
	change_x = change_xaxis(direction);
	
	set_position(12, 0);
	print_string("Username");
	revert_colour_line(0);
	
	set_position(4,4);
	
	while (1)
	{
		position = read_joystick_position(calibration);
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
		
		set_position(10,4);
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
			for(int i=0; i<SRAM_USERNAME_ADDR_LENGTH;i++)
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
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_new_game(JoystickPosition calibration)
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y, change_x;
	ChangeTouchpadData change_touch_data;
	TouchpadData touch_data;
	char scr_position = 0;
	
	position = read_joystick_position(calibration);
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
		position = read_joystick_position(calibration);
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
					return State_Tutorial;
				case 1:
					return State_Easy;
				case 2:
					return State_Normal;
				case 3:
					return State_Hard;
				case 4:
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
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_high_scores(JoystickPosition calibration)
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_x;
	char scr_position = 0;
	volatile char *eeprom = (char *) EEPROM_HIGH_SCORES_BASE_ADDR;
	
	position = read_joystick_position(calibration);
	direction = read_joystick_direction(position);
	change_x = change_xaxis(direction);
	
	set_position(9,0);
	print_string("High Scores");
	revert_colour_line(0);
	
	/*
	for(int j = 0; j < EEPROM_HIGH_SCORES_ADDR_LEN; j)
	{
		for (int k = 0; k < 5; k++)
		{
			eeprom_busy_wait();
			printf("%c", eeprom_read_byte(eeprom[j + k]));
		}
		eeprom_busy_wait();
		printf(": %d\n", eeprom_read_dword(eeprom[j + 5]));
	}*/
	
	refresh_oled();
	
	while(1)
	{
		position = read_joystick_position(calibration);
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
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_options(JoystickPosition calibration)
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y, change_x;
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	char scr_position = 0;
	
	position = read_joystick_position(calibration);
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
		position = read_joystick_position(calibration);
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
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_mode(JoystickPosition calibration)
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y;
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	char scr_position = 0;
	
	position = read_joystick_position(calibration);
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
		position = read_joystick_position(calibration);
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
					printf("Normal\n");
					break;
				case 1:
					printf("Reverse\n");
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
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_sound(JoystickPosition calibration)
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y;
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	char scr_position = 0;
	
	position = read_joystick_position(calibration);
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
		position = read_joystick_position(calibration);
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
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_tutorial(JoystickPosition calibration)
{
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	set_position(14,0);
	print_string("Tutorial");
	revert_colour_line(0);
	refresh_oled();
	while(1)
	{
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			return State_NewGame;
		}
	}
}

/****************************************************************************
* \brief Function call during the easy mode gameplay
*
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_easy(JoystickPosition calibration)
{
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	set_position(14,0);
	print_string("Easy");
	revert_colour_line(0);
	refresh_oled();
	while(1)
	{
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			return State_NewGame;
		}
	}
}

/****************************************************************************
* \brief Function call during the normal mode gameplay
*
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_normal(JoystickPosition calibration)
{
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	set_position(12,0);
	print_string("Normal");
	revert_colour_line(0);
	refresh_oled();
	while(1)
	{
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			return State_NewGame;
		}
	}
}

/****************************************************************************
* \brief Function call during the hard mode gameplay
*
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_hard(JoystickPosition calibration)
{
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	set_position(12,0);
	print_string("Hard");
	revert_colour_line(0);
	refresh_oled();
	while(1)
	{
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			return State_NewGame;
		}
	}
}

/****************************************************************************
* \brief Function call during the insane mode gameplay
*
* \param in joystick calibration data
****************************************************************************/
InterfaceState interface_insane(JoystickPosition calibration)
{
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	set_position(12,0);
	print_string("Insane");
	revert_colour_line(0);
	refresh_oled();
	while(1)
	{
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
		if (change_touch_data.rightButton)
		{
			
			
			return State_NewGame;
		}
	}
}
