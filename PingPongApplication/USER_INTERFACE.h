/*
 * USER_INTERFACE.h
 *
 * Created: 17.09.2015 15:28:19
 *  Author: michalma
 */ 

#include "OLED_DRIVER.h"
#include "JOYSTICK_DRIVER.h"
#include "COM_LIB.h"
#include "TIME_DRIVER.h"
#include <util/delay.h>


#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_


typedef enum
{
	State_Username,
	State_NewGame,
	State_Tutorial,
	State_Easy,
	State_Normal,
	State_Hard,
	State_Insane,
	State_Options,
	State_HighScores,
	State_Clear_HighScores,
	State_Mode,
	State_Playing,
	State_Endgame
} InterfaceState;

void interface_init();
void interface_state_machine();
InterfaceState interface_username();
InterfaceState interface_new_game();
InterfaceState interface_high_scores();
InterfaceState interface_options();
InterfaceState interface_clear_highscores();
InterfaceState interface_mode();
InterfaceState interface_tutorial();
InterfaceState interface_print_mode(GameModes mode);
InterfaceState interface_playing();
InterfaceState inteface_end_game();

uint8_t minigame_push_buttons();
uint8_t minigame_slide_left_slider_left();
uint8_t minigame_slide_both_sliders_apart();
uint8_t minigame_push_left_button_7x();
uint8_t minigame_buttons_in_random_order(uint8_t order);

#endif /* USER_INTERFACE_H_ */