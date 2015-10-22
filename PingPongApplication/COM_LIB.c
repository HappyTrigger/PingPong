/*
 * COM_LIB.c
 *
 * Created: 15.10.2015 12:55:14
 *  Author: michalma
 */ 
#include "COM_LIB.h"
#include "CAN_DRIVER.h"
#include "JOYSTICK_DRIVER.h"


uint8_t send_joystick_possition(JoystickPosition joystick_position)
{
	CANMessage message;
	message.ID = 0x01;
	message.length = 2;
	
	message.data_array[0] = (uint8_t) (joystick_position.xaxis);	
	message.data_array[1] = (uint8_t) (joystick_position.yaxis);	
	
	return CAN_send_message(message);
}

uint8_t send_touchpad_possition(TouchpadData touchpad_data)
{
	CANMessage message;
	message.ID = 0x02;
	message.length = 2;
	
	message.data_array[0] = (uint8_t) (touchpad_data.rightTouchPad);
	message.data_array[1] = (uint8_t) (touchpad_data.leftTouchPad);
	
	return CAN_send_message(message);
}

uint8_t send_buttons_status(TouchpadData touchpad_data)
{
	CANMessage message;
	message.ID = 0x03;
	message.length = 2;
	
	message.data_array[0] = (uint8_t) (touchpad_data.rightButton);
	message.data_array[1] = (uint8_t) (touchpad_data.leftButton);
	
	
	return CAN_send_message(message);
}
