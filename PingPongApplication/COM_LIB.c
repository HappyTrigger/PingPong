/*
 * COM_LIB.c
 *
 * Created: 15.10.2015 12:55:14
 *  Author: michalma
 */ 
#include "COM_LIB.h"
#include "CAN_DRIVER.h"
#include "JOYSTICK_DRIVER.h"


uint8_t send_joystick_possition(JoystickPosition joystick_position, TouchpadData touchpad_data)
{
	CANMessage message;
	message.ID = 0x01;
	message.length = 6;
	
	message.data_array[0] = (uint8_t) (joystick_position.xaxis);	
	message.data_array[1] = (uint8_t) (joystick_position.yaxis);
	
	message.data_array[2] = (uint8_t) (touchpad_data.rightTouchPad);
	message.data_array[3] = (uint8_t) (touchpad_data.leftTouchPad);
	
	message.data_array[4] = (uint8_t) (touchpad_data.rightButton);
	message.data_array[5] = (uint8_t) (touchpad_data.leftButton);
	
	return CAN_send_message(message);
}


uint8_t send_game_mode(GameModes mode){
	CANMessage message;
	message.ID = 0x02;
	message.length = 1;
	
	message.data_array[0]= (uint8_t) (mode);
	
	return CAN_send_message(message);
}