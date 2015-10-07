/*
 * USER_INTERFACE.h
 *
 * Created: 17.09.2015 15:28:19
 *  Author: michalma
 */ 

#include "OLED_DRIVER.h"
#include "JOYSTICK_DRIVER.h"


#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_

#define EEPROM_HIGH_SCORES_BASE_ADDR 0x00
#define EEPROM_HIGH_SCORES_ADDR_LEN 5


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
	State_Sound,
	State_Mode
} InterfaceState;

void interface_init();

void interface_state_machine(JoystickPosition calibration); 
InterfaceState interface_username(JoystickPosition calibration);
InterfaceState interface_new_game(JoystickPosition calibration);
InterfaceState interface_high_scores(JoystickPosition calibration);
InterfaceState interface_options(JoystickPosition calibration);
InterfaceState interface_sound(JoystickPosition calibration);
InterfaceState interface_mode(JoystickPosition calibration);
InterfaceState interface_tutorial(JoystickPosition calibration);
InterfaceState interface_easy(JoystickPosition calibration);
InterfaceState interface_normal(JoystickPosition calibration);
InterfaceState interface_hard(JoystickPosition calibration);
InterfaceState interface_insane(JoystickPosition calibration);





#endif /* USER_INTERFACE_H_ */