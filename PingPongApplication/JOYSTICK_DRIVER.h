/*
 * IncFile2.h
 *
 * Created: 10.09.2015 14:40:43
 *  Author: haakongn
 */ 


#ifndef JOYSTICK_DRIVER_H_
#define JOYSTICK_DRIVER_H_

typedef struct JoystickPosition
{
	volatile int xaxis;
	volatile int yaxis;
} JoystickPosition;


typedef struct TouchpadData{
	volatile int rightTouchPad;
	volatile int leftTouchPad;
	volatile char rightButton;
	volatile char leftButton;
}TouchpadData;

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


#endif /* JOYSTICK_DRIVER_H_ */