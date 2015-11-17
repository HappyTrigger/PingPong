/*
 * TIME_DRIVER.c
 *
 * Created: 14.11.2015 15:26:13
 *  Author: haakongn
 */ 

#include "TIME_DRIVER.h"

void timer1_init()
{
	TCCR1A = 0x00; // normal mode, no force output, CTC mode
	TCCR1B = 0b00001101; // no noise canceling, CTC mode, 1024 prescaler
	TCNT1 = 0x00; // value of the timer/counter
	OCR1A = 4800; // interrupt trigger value (trigger frequency is 0.999Hz)
	TIMSK |= 0b01000000; // interrupt enable
}

void timer1_reset()
{
	current_time = 0;
}

uint16_t timer1_get_time()
{
	return current_time;
}

ISR(TIMER1_COMPA_vect)
{
	cli();
	current_time++;
	TCNT1 = 0x00;
	led_toggle();
	sei();
}