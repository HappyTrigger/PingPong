/*
 * COM_LIB.h
 *
 * Created: 15.10.2015 13:43:17
 *  Author: michalma
 */ 


#ifndef COM_LIB_H_
#define COM_LIB_H_

#include <stdint.h>
#include "CAN_DRIVER.h"

typedef struct JoystickPosition
{
	volatile uint8_t xaxis;
	volatile uint8_t yaxis;
} JoystickPosition;

typedef struct TouchpadData
{
  volatile uint8_t rightTouchPad;
  volatile uint8_t leftTouchPad;
  volatile uint8_t rightButton;
  volatile uint8_t leftButton;
} TouchpadData;

uint8_t receive_and_decode_message(JoystickPosition* joystick_position, TouchpadData* touchpad_data);
uint8_t receive_joistick_position(JoystickPosition* joystick_position, CANMessage message);
uint8_t receive_touchpad_data(TouchpadData* touchpad_data, CANMessage message);
uint8_t receive_button_data(TouchpadData* touchpad_data, CANMessage message);
JoystickPosition Joystick_pwm(JoystickPosition* joystick_position);


#endif /* COM_LIB_H_ */
