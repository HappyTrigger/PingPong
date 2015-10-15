/*
 * COM_LIB.h
 *
 * Created: 15.10.2015 12:54:47
 *  Author: michalma
 */ 


#ifndef COM_LIB_H_
#define COM_LIB_H_
#include <stdint.h>

uint8_t send_joystick_possition();

uint8_t send_touchpad_possition();

uint8_t send_buttons_status();


#endif /* COM_LIB_H_ */