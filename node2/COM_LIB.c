/*
 * COM_LIB.c
 *
 * Created: 15.10.2015 13:43:31
 *  Author: michalma
 */
#include "COM_LIB.h"
#include "MCP_ADDRESSES.h"



uint8_t receive_and_decode_message(JoystickPosition* joystick_position, TouchpadData* touchpad_data, Mode* game_mode)
{
  uint8_t tmp;
  CANMessage message;
  tmp = CAN_receive_message(&message);
  if (tmp == SUCCESS)
  {
    switch(message.ID)
    {
      case 0x01:
        receive_joistick_position(joystick_position, touchpad_data, message);
        break;
      case 0x02:
        receive_mode_change(game_mode, message);
        break;
       default:
        break;
    }
  }
  return tmp;
}


uint8_t receive_joistick_position(JoystickPosition* joystick_position, TouchpadData* touchpad_data, CANMessage message)
{
  joystick_position->xaxis = message.data_array[0];
  joystick_position->yaxis = message.data_array[1];

  touchpad_data->rightTouchPad = message.data_array[2];
  touchpad_data->leftTouchPad = message.data_array[3];
  
  touchpad_data->rightButton = message.data_array[4];
  touchpad_data->leftButton = message.data_array[5];
  
}

uint8_t receive_mode_change(Mode* CurrentMode, CANMessage message){
  CurrentMode->gamemode = message.data_array[0];
}

