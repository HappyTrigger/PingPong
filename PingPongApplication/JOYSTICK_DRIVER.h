/*
 * IncFile2.h
 *
 * Created: 10.09.2015 14:40:43
 *  Author: haakongn
 */ 


#ifndef JOYSTICK_DRIVER_H_
#define JOYSTICK_DRIVER_H_

#include <stdint.h>

#define BUTTON_PORT PIND
#define RIGHT_BUTTON_PIN PD4
#define LEFT_BUTTON_PIN PD5

/****************************************************************************
* \brief Structure holding data about joystick position
*
****************************************************************************/
typedef struct JoystickPosition
{
	volatile uint8_t xaxis;
	volatile uint8_t yaxis;
} JoystickPosition;

/****************************************************************************
* \brief Structure holding data from touchpad
*
****************************************************************************/
typedef struct TouchpadData
{
	volatile uint8_t rightTouchPad;
	volatile uint8_t leftTouchPad;
	volatile uint8_t rightButton;
	volatile uint8_t leftButton;
} TouchpadData;

typedef struct ChangeTouchpadData
{
	uint8_t rightButton;
	uint8_t leftButton;
} ChangeTouchpadData;


typedef enum
{
	Left,
	Right,
	Up,
	Down,
	Neutral,
} JoystickDirection;

JoystickPosition read_joystick_position();
JoystickDirection read_joystick_direction(JoystickPosition joystickPos);
JoystickPosition joystick_calibration();
TouchpadData read_touchpad_data();

JoystickDirection change_xaxis(JoystickDirection direction);
JoystickDirection change_yaxis(JoystickDirection direction);
ChangeTouchpadData change_touchpad_data(TouchpadData new_touchpad_data);

#endif /* JOYSTICK_DRIVER_H_ */