// CONTROL_DRIVER.h

#ifndef _CONTROL_DRIVER_h
#define _CONTROL_DRIVER_h

#include "arduino.h"
#include "COM_LIB.h"

#define MJ1_OE A7
#define MJ1_RST A6
#define MJ1_SEL A5
#define MJ1_EN A4
#define MJ1_DIR A3

#define DIR_RIGHT HIGH
#define DIR_LEFT LOW

#define MJ2_PORT PORTK
#define MJ2_PIN PINK
#define MJ2_DDR DDRK




void control_init();

void speed_controller(uint16_t speed_value, uint8_t motor_direction);

uint8_t reverse_byte_input(uint8_t input);

void encoder_calibration();

int16_t return_max();

int16_t read_encoder();

int position_controller(uint8_t position_value);

void joystick_position_controller(JoystickPosition* joystick_position);


#endif
//#endif



