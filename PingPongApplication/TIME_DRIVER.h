/*
 * TIME_DRIVER.h
 *
 * Created: 14.11.2015 15:25:55
 *  Author: haakongn
 */ 


#ifndef TIME_DRIVER_H_
#define TIME_DRIVER_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include "PING_PONG_LIB.h"

volatile static uint16_t current_time;

void timer1_init();
void timer1_reset();
uint16_t timer1_get_time();
ISR(TIMER1_COMPA_vect);


#endif /* TIME_DRIVER_H_ */