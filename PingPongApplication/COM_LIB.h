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

typedef enum
{
	Tutorial,
	Easy,
	Normal,
	Hard,
	Insane,
	Reverse_settings,
	Normal_settings
} GameModes;

uint8_t send_joystick_possition(JoystickPosition joystick_position, TouchpadData touchpad_data);

uint8_t send_game_mode(GameModes mode);
/*
uint8_t send_touchpad_possition();

uint8_t send_buttons_status();
*/

#endif /* COM_LIB_H_ */