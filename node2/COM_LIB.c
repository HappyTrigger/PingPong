/*
 * COM_LIB.c
 *
 * Created: 15.10.2015 13:43:31
 *  Author: michalma
 */
#include "COM_LIB.h"
#include "CAN_DRIVER.h"
#include "MCP_ADDRESSES.h"


uint8_t receive_joistick_position(JoystickPosition* joystick_position)
{

	uint8_t tmp;
	CANMessage message;
	tmp = CAN_receive_message(&message);
	if (tmp == SUCCESS)
	{
		joystick_position->xaxis = message.data_array[0];
		joystick_position->xaxis |= ((int) message.data_array[1]) >> 8;
		joystick_position->xaxis |= ((int) message.data_array[2]) >> 16;
		joystick_position->xaxis |= ((int) message.data_array[3]) >> 24;
		
		joystick_position->yaxis = message.data_array[4];
		joystick_position->yaxis |= ((int) message.data_array[5]) >> 8;
		joystick_position->yaxis |= ((int) message.data_array[6]) >> 16;
		joystick_position->yaxis |= ((int) message.data_array[7]) >> 24;


	}
	
	return tmp;
}


JoystickPosition Joystick_pwm(JoystickPosition* joystick_position){
	int xaxistemp=joystick_position->xaxis;
	int yaxistemp=joystick_position->yaxis;
	
	xaxistemp = ((((xaxistemp-127)*100)/255)-50)*2;
	yaxistemp= ((((yaxistemp-127)*100)/255)-50)*2;
	
	joystick_position->xaxis = xaxistemp;
	joystick_position->yaxis =yaxistemp;
}