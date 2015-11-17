/*
 * COM_LIB.c
 *
 * Created: 15.10.2015 13:43:31
 *  Author: michalma
 */
#include "COM_LIB.h"
#include "MCP_ADDRESSES.h"
#include <Arduino.h>


/****************************************************************************
* \brief Receive and decode message from CAN
*
* \param out joystick position
* \param out touchpad data
* \param out game mode
*
* \return result of the CAN receive message operation
****************************************************************************/
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

/****************************************************************************
* \brief Decode received message and fill in the structures for joystick 
*		 position and touchpad data
*
* \param out joystick position
* \param out touchpad data
* \param in received CAN message
*
****************************************************************************/
void receive_joistick_position(JoystickPosition* joystick_position, TouchpadData* touchpad_data, CANMessage message)
{
  joystick_position->xaxis = message.data_array[0];
  joystick_position->yaxis = message.data_array[1];

  touchpad_data->rightTouchPad = message.data_array[2];
  touchpad_data->leftTouchPad = message.data_array[3];
  
  touchpad_data->rightButton = message.data_array[4];
  touchpad_data->leftButton = message.data_array[5];
  
}

/****************************************************************************
* \brief Decode received message and fill in the structures for game mode
*
* \param out game mode
* \param in received CAN message
*
****************************************************************************/
void receive_mode_change(Mode* CurrentMode, CANMessage message)
{
  CurrentMode->gamemode = message.data_array[0];
}

/***********************************************************************************
* \brief Return the change in the right touch button
*
* \return True if the touch button has been pushed
***********************************************************************************/
uint8_t change_touchpad_data(uint8_t new_right_button_status)
{
  static uint8_t old_right_button_status;
  uint8_t ret_val;
  
  ret_val = 0;
    
  if (old_right_button_status == 0 && new_right_button_status == 1)
  {
    ret_val = 1;
  }
  old_right_button_status = new_right_button_status;
  
  return ret_val;
}


uint8_t end_game()
{
	CANMessage message;
	message.ID = 0x05;
	message.length = 0;
	
	
	return CAN_send_message(message);
}

uint8_t request_minigame()
{
	CANMessage minigame_message;
	minigame_message.length = 0;
	
	uint8_t random_game = random(0,5); 
	uint8_t random_value = random(0, 255);
	
	switch (random_game)
	{
		case 0:
			minigame_message.ID = 0x06;
			break;
		case 1:
			minigame_message.ID = 0x07;
			break;
		case 2:
			minigame_message.ID = 0x08;
			break;
		case 3:
			minigame_message.ID = 0x09;
			break;
		case 4:
			minigame_message.ID = 0x0A;
			minigame_message.length = 1;
			minigame_message.data_array[0] = random_value;
			break;

	}
	return CAN_send_message(minigame_message);
}