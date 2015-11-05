// CONTROL_DRIVER.h

#ifndef _CONTROL_DRIVER_h
#define _CONTROL_DRIVER_h

#include "arduino.h"

#define MJ1_OE A7
#define MJ1_RST A6
#define MJ1_SEL A5
#define MJ1_EN A4
#define MJ1_DIR A3

#define DIR_RIGHT LOW
#define DIR_LEFT HIGH

#define MJ2_PORT PORTK
#define MJ2_PIN PINK
#define MJ2_DDR DDRK

void control_init();

int8_t speed_controller(int8_t speed_value);

uint8_t reverse_byte_input(uint8_t input);

void encoder_calibration();

int16_t return_max();

int16_t read_encoder();

void position_controller(uint8_t position_value)


#endif
//#endif



