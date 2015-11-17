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

static uint16_t current_score;
static GameModes current_mode;

/****************************************************************************
* \brief Initialize the user interface
*
****************************************************************************/
void interface_init()
{
	volatile char *ext_ram = (char *) SRAM_USERNAME_ADDR;
	uint8_t i;
	
	for(i = 0; i < USERNAME_LENGTH; i++)
	{
		ext_ram[i] = 'A';
	}
	
	SRAM_high_score_write();
	
}


/****************************************************************************
* \brief The main function serving the game
*

****************************************************************************/
void interface_state_machine()
{
	static InterfaceState state = State_Username;
	
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
		
		case State_Clear_HighScores :
		state = interface_clear_highscores();
		break;
		
		case  State_Mode :
		state = interface_mode();
		break;
		
		case State_Playing:
		state = interface_playing();
		break;
		
		case State_Endgame:
		state = inteface_end_game();
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
	uint8_t index = 0;
	uint8_t i;
	
	clear_oled();
	
	for(i=0; i<USERNAME_LENGTH;i++)
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
		for (i = 0; i < USERNAME_LENGTH; i++)
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
			for(i = 0; i < USERNAME_LENGTH; i++)
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
	uint8_t scr_position = 0;
	
	clear_oled();
	
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
			scr_position = scr_position == 0 ? 4 : scr_position - 1;
		}
		else if (change_y == Down)
		{
			set_position(1, (scr_position % 5) + 2);
			print_char(' ');
			scr_position = scr_position == 4 ? 0 : scr_position + 1;
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
****************************************************************************/
InterfaceState interface_high_scores()
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_x;
	uint8_t i, j;
	volatile char* high_scores_sram = (char*) SRAM_HIGH_SCORES_ADDR;
	
	clear_oled();
	
	position = read_joystick_position();
	direction = read_joystick_direction(position);
	change_x = change_xaxis(direction);
	
	set_position(9,0);
	print_string("High Scores");
	revert_colour_line(0);
	
	for(i = 0; i < (HIGH_SCORES_TABLE_LENGTH); i ++)
	{
		set_position(8, i + 2);
		for (j = 0; j < SRAM_HIGH_SCORE_LENGTH; j++)
		{
			print_char(high_scores_sram[i * SRAM_HIGH_SCORE_LENGTH + j]);
		}
	}
	
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
	uint8_t scr_position = 0;
	
	clear_oled();
	
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
	print_string("Clear HighScore");
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
			scr_position = scr_position == 0 ? 2 : scr_position - 1;
		}
		else if (change_y == Down)
		{
			set_position(1, (scr_position % 3) + 3);
			print_char(' ');
			scr_position = scr_position == 2 ? 0 : scr_position + 1;
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
				return State_Clear_HighScores;
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
	uint8_t scr_position = 0;
	
	clear_oled();
	
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
			scr_position = scr_position == 0 ? 1 : 0;
		}
		else if (change_y == Down)
		{
			set_position(1, (scr_position % 2) * 2 + 3);
			print_char(' ');
			scr_position = scr_position == 1 ? 0 : 1;
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
				send_game_mode(Reverse_settings);
				printf("Normal\n");
				break;
				case 1:
				send_game_mode(Normal_settings);
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

****************************************************************************/
InterfaceState interface_clear_highscores()
{
	JoystickPosition position;
	JoystickDirection direction;
	JoystickDirection change_y;
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	uint8_t scr_position = 0;
	
	clear_oled();
	
	position = read_joystick_position();
	direction = read_joystick_direction(position);
	change_y = change_yaxis(direction);

	set_position(7,0);
	print_string("Clear HighScores");
	revert_colour_line(0);
	set_position(3,3);
	print_string("YES");
	set_position(3,5);
	print_string("NO");
	
	while(1)
	{
		position = read_joystick_position();
		direction = read_joystick_direction(position);
		change_y = change_yaxis(direction);
		
		if (change_y == Up)
		{
			set_position(1, (scr_position % 2) * 2 + 3);
			print_char(' ');
			scr_position = scr_position == 0 ? 1 : 0;
		}
		else if (change_y == Down)
		{
			set_position(1, (scr_position % 2) * 2 + 3);
			print_char(' ');
			scr_position = scr_position == 1 ? 0 : 1;
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
					high_score_clear();
					SRAM_high_score_write();
					break;
				case 1:
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
	
	clear_oled();
	
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
		position.yaxis = 137;
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
	current_mode = mode;
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
		
		default :
		break;
	}
	revert_colour_line(0);
	//refresh_oled();
	
	return State_Playing;
	
}


/****************************************************************************
* \brief Main function running while playing the game
*
* 
****************************************************************************/
InterfaceState interface_playing()
{
	CANMessage canMessageNode2;
	TouchpadData touch_data;
	ChangeTouchpadData change_touch_data;
	JoystickPosition position;
	char buffer[9];
	uint16_t old_time = 0;
	uint16_t current_time = 0;
	uint8_t multiplier = 0;
	uint8_t pl = 0;
	
	switch(current_mode)
	{
		case Easy:
			multiplier = 1;
			break;
			
		case Normal:
			multiplier = 2;
			break;
			
		case Hard:
			multiplier = 3;
			break;
			
		case Insane:
			multiplier = 4;
			break;
		
		default:
			multiplier = 1;
	}
	
	clear_oled();
	interface_print_mode(current_mode);
	set_position(13,4);
	print_string("SHOOT");
	refresh_oled();
	
	touch_data = read_touchpad_data();
	change_touch_data = change_touchpad_data(touch_data);
	
	while (!change_touch_data.rightButton)
	{
		touch_data = read_touchpad_data();
		change_touch_data = change_touchpad_data(touch_data);
	}
	
	set_position(13,4);
	print_string("     ");
	refresh_oled();
	
	timer1_reset();
	current_time = old_time = timer1_get_time();
	
	do
	{
		position = read_joystick_position();
		touch_data = read_touchpad_data();
		
		send_joystick_possition(position, touch_data);
		_delay_ms(1);
		
		current_time = timer1_get_time();

		if (current_time != old_time)
		{
			clear_oled();
			interface_print_mode(current_mode);
			current_score = old_time * multiplier;
			set_position(13,2);
			print_string("Time");
			snprintf(buffer, 9, "%02d:%02d:%02d",old_time / 3600 , old_time / 60, old_time % 60);
			set_position(11,3);
			print_string(buffer);
			set_position(13,4);
			print_string("Score"); 
			snprintf(buffer, 9, "%05d",current_score);
			set_position(13,5);
			print_string(buffer);
			
			refresh_oled();
			old_time = current_time;
		}
		
		if(CAN_receive_message(&canMessageNode2) != SUCCESS)
		{
			canMessageNode2.ID = 0xFF;
		}
		else
		{
			switch (canMessageNode2.ID)
			{
				case 0x06:
					minigame_push_buttons();
					break;

				case 0x07:
					minigame_slide_left_slider_left();
					break;
					
				case 0x08:
					minigame_slide_both_sliders_apart();
					break;
					
				case 0x09:
					minigame_push_left_button_7x();
					break;
					
				case 0x0A:
					minigame_buttons_in_random_order(canMessageNode2.data_array[0]);
					break;
			}
		}
		
	} while(canMessageNode2.ID != 0x05);//current_time <= 10);//canMessageNode2.ID != 0x05);
	
	return State_Endgame;
}


/****************************************************************************
* \brief Animations and high-scores handling
*
* 
****************************************************************************/
InterfaceState inteface_end_game()
{
	uint8_t position;
	uint16_t old_time;
	volatile char *ext_ram = (char *) SRAM_USERNAME_ADDR;
	char name[USERNAME_LENGTH + 1];
	char position_info[11];
	uint8_t i;
	
	for(i = 0; i < USERNAME_LENGTH; i++)
	{
		name[i] = ext_ram[i];
	}
	name[USERNAME_LENGTH] = '\0';
	
	position = high_score_add(name, current_score);
	
	end_game_animation(0);
	clear_oled();
	set_position(10,3);
	print_string("GAME OVER!");
	if (position)
	{
		set_position(10,4);
		snprintf(position_info, 11, "You are #%d", position);
		print_string(position_info);
	}
	
	refresh_oled();
	
	old_time = current_time = timer1_get_time();
	old_time += 3;
	while(current_time <= old_time)
	{
		current_time = timer1_get_time();
	}
	end_game_animation(1);
	
	return State_HighScores;
}

uint8_t minigame_push_buttons()
{
	TouchpadData touch_data;
	
	clear_oled();
	set_position(7,3);
	print_string("PUSH BOTH BUTTONS!");
	refresh_oled();
	
	while (1)
	{	
		touch_data = read_touchpad_data();
		if (touch_data.leftButton && touch_data.rightButton)
		{
			return 1;
		}
	}
	return 0;
}
uint8_t minigame_slide_left_slider_left()
{
	TouchpadData touch_data;
	uint8_t been_right = 0;
	
	clear_oled();
	set_position(4,3);
	print_string("SLIDE LEFT SLIDER FROM");
	set_position(4,4);
	print_string("THE RIGHT TO THE LEFT!");
	refresh_oled();
	
	while (1)
	{
		touch_data = read_touchpad_data();
		if (!been_right && touch_data.leftTouchPad > 220)
		{
			been_right = 1;
		}
		
		if (been_right && touch_data.leftTouchPad < 20)
		{
			return 1;
		}
	}
	return 0;
}
uint8_t minigame_slide_both_sliders_apart()
{
	TouchpadData touch_data;
	uint8_t left_been_right = 0;
	uint8_t right_been_left = 0;
	
	clear_oled();
	set_position(3,3);
	print_string("SLIDE BOTH SLIDERS APART!");
	refresh_oled();
	
	while (1)
	{
		touch_data = read_touchpad_data();
		if (!left_been_right && touch_data.leftTouchPad > 220)
		{
			left_been_right = 1;
		}
		
		if (!right_been_left && touch_data.rightTouchPad < 20)
		{
			right_been_left = 1;
		}
		
		if (left_been_right && right_been_left && (touch_data.leftTouchPad < 20) && (touch_data.rightTouchPad > 220))
		{
			return 1;
		}
	}
	return 0;
}
uint8_t minigame_push_left_button_7x()
{
	TouchpadData touch_data;
	ChangeTouchpadData change_data;
	uint8_t count = 7;
	
	clear_oled();
	set_position(5,3);
	print_string("PUSH LEFT BUTTON 7x!");
	refresh_oled();
	
	while (count)
	{
		touch_data = read_touchpad_data();
		change_data = change_touchpad_data(touch_data);
		if (change_data.leftButton)
		{
			count--;
		}
	}
	return 1;
}
uint8_t minigame_buttons_in_random_order(uint8_t order)
{
	TouchpadData touch_data;
	ChangeTouchpadData change_data;
	
	clear_oled();
	set_position(10,2);
	print_string("PUSH BUTTONS!");
	refresh_oled();
	
	order &= 0x1F;
	order |= 0x20;
	
	while (order != 1)
	{
		if (!(order & 0x01))
		{
			set_position(15,4);
			print_string("LEFT ");
			refresh_oled();
			do
			{
				touch_data = read_touchpad_data();
				change_data = change_touchpad_data(touch_data);
			}
			while (!change_data.leftButton);
		}
		else
		{
			set_position(15,4);
			print_string("RIGHT");
			refresh_oled();
			do
			{
				touch_data = read_touchpad_data();
				change_data = change_touchpad_data(touch_data);
			}
			while (!change_data.rightButton);
		}
		order >>= 1;
		order &= 0x7F;
	}
	return 1;	
}