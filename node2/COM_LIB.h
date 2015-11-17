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

typedef enum
{
  No_mode,
  Tutorial,
  Easy,
  Normal,
  Hard,
  Insane,
  Settings_normal,
  Settings_reverse,
  Endgame
} GameModes;

typedef struct Mode
{
  volatile uint8_t gamemode;
}Mode;

uint8_t receive_and_decode_message(JoystickPosition* joystick_position, TouchpadData* touchpad_data, Mode* game_mode);
void receive_joistick_position(JoystickPosition* joystick_position, TouchpadData* touchpad_data, CANMessage message);
void receive_mode_change(Mode* CurrentMode, CANMessage message);

uint8_t change_touchpad_data(uint8_t new_right_button_status);
uint8_t end_game();
uint8_t request_minigame();

#endif /* COM_LIB_H_ */
