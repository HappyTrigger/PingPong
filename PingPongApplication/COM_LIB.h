/*
 * COM_LIB.h
 *
 * Created: 15.10.2015 12:54:47
 *  Author: michalma
 */ 


#ifndef COM_LIB_H_
#define COM_LIB_H_
#include <stdint.h>
#include "JOYSTICK_DRIVER.h"
#include "CAN_DRIVER.h"

typedef enum
{
	Tutorial=1,
	Easy,
	Normal,
	Hard,
	Insane,
	Normal_settings,
	Reverse_settings,
	Endgame
} GameModes;

uint8_t send_joystick_possition(JoystickPosition joystick_position, TouchpadData touchpad_data);

uint8_t send_game_mode(GameModes mode);


#endif /* COM_LIB_H_ */