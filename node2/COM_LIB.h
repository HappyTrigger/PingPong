/*
 * COM_LIB.h
 *
 * Created: 15.10.2015 13:43:17
 *  Author: michalma
 */ 


#ifndef COM_LIB_H_
#define COM_LIB_H_

#include <stdint.h>

typedef struct JoystickPosition
{
	volatile uint8_t xaxis;
	volatile uint8_t yaxis;
} JoystickPosition;

uint8_t receive_joistick_position(JoystickPosition* joystick_position);
JoystickPosition Joystick_pwm(JoystickPosition* joystick_position);


#endif /* COM_LIB_H_ */
