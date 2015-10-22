/*
 * COM_LIB.c
 *
 * Created: 15.10.2015 13:43:31
 *  Author: michalma
 */
#include "COM_LIB.h"
#include "MCP_ADDRESSES.h"



uint8_t receive_and_decode_message(JoystickPosition* joystick_position, TouchpadData* touchpad_data)
{
  uint8_t tmp;
  CANMessage message;
  tmp = CAN_receive_message(&message);
  if (tmp == SUCCESS)
  {
    switch(message.ID)
    {
      case 0x01:
        receive_joistick_position(joystick_position, message);; 
        break;
      case 0x02:
        receive_touchpad_data(touchpad_data, message);
        break;
      case 0x03:
        receive_button_data(touchpad_data, message);
        break;
       default:
        break;
    }
  }
  return tmp;
}


uint8_t receive_joistick_position(JoystickPosition* joystick_position, CANMessage message)
{
  joystick_position->xaxis = message.data_array[0];
	//joystick_position->xaxis |= (message.data_array[1]) >> 8;
	//joystick_position->xaxis |= (message.data_array[2]) >> 16;  
  //joystick_position->xaxis |= (message.data_array[3]) >> 24;
		
	joystick_position->yaxis = message.data_array[1];
	//joystick_position->yaxis |= (message.data_array[5]) >> 8;
	//joystick_position->yaxis |= (message.data_array[6]) >> 16;
	//joystick_position->yaxis |= (message.data_array[7]) >> 24;
}

uint8_t receive_touchpad_data(TouchpadData* touchpad_data, CANMessage message)
{
  touchpad_data->rightTouchPad = message.data_array[0];
 // touchpad_data->rightTouchPad |= (int)(message.data_array[1]) >> 8;
 // touchpad_data->rightTouchPad |= (int)(message.data_array[2]) >> 16;
 // touchpad_data->rightTouchPad |= (int)(message.data_array[3]) >> 24;
    
  touchpad_data->leftTouchPad = message.data_array[1];
 // touchpad_data->leftTouchPad |= (message.data_array[5]) >> 8;
 // touchpad_data->leftTouchPad |= (message.data_array[6]) >> 16;
 // touchpad_data->leftTouchPad |= (message.data_array[7]) >> 24;
}

uint8_t receive_button_data(TouchpadData* touchpad_data, CANMessage message)
{
  touchpad_data->rightButton = message.data_array[0];
  touchpad_data->leftButton = message.data_array[1];
}


JoystickPosition Joystick_pwm(JoystickPosition* joystick_position)
{
	int xaxistemp=joystick_position->xaxis;
	int yaxistemp=joystick_position->yaxis;
	
	xaxistemp = ((((xaxistemp-127)*100)/255)-50)*2;
	yaxistemp= ((((yaxistemp-127)*100)/255)-50)*2;
	
	joystick_position->xaxis = xaxistemp;
	joystick_position->yaxis =yaxistemp;
}
