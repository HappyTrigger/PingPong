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
	message.length = 8;
	
	message.data_array[0] = (uint8_t) (joystick_position.xaxis);
	message.data_array[1] |= (uint8_t) (joystick_position.xaxis << 8);
	message.data_array[2] |= (uint8_t) (joystick_position.xaxis << 16);
	message.data_array[3] |= (uint8_t) (joystick_position.xaxis << 24);
	message.data_array[3] |= (uint8_t) (joystick_position.xaxis << 24);
	
	message.data_array[4] = (uint8_t) (joystick_position.yaxis);
	message.data_array[5] |= (uint8_t) (joystick_position.yaxis << 8);
	message.data_array[6] |= (uint8_t) (joystick_position.yaxis << 16);
	message.data_array[7] |= (uint8_t) (joystick_position.yaxis << 24);
	
	
	/*
	message.data_array[0] = (uint8_t) (joystick_position.xaxis & 0xFF);
	message.data_array[1] = (uint8_t) (joystick_position.xaxis & 0xFF00);
	message.data_array[2] = (uint8_t) (joystick_position.xaxis & 0xFF0000);
	message.data_array[3] = (uint8_t) (joystick_position.xaxis & 0xFF000000);
	message.data_array[4] = (uint8_t) (joystick_position.yaxis & 0xFF);
	message.data_array[5] = (uint8_t) (joystick_position.yaxis & 0xFF00);
	message.data_array[6] = (uint8_t) (joystick_position.yaxis & 0xFF0000);
	message.data_array[7] = (uint8_t) (joystick_position.yaxis & 0xFF000000);
	*/
	return CAN_send_message(message);
}

uint8_t send_touchpad_possition()
{

}

uint8_t send_buttons_status()
{
	
}
