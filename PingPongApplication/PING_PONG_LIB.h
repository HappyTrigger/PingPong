#ifndef _PING_PONG_LIBRARY_
#define _PING_PONG_LIBRARY_

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "USART_DRIVER.h"

#define BLINK_TIME 1000

#define OLED_COMMAND_BASE_ADDR 0x1000
#define OLED_DATA_BASE_ADDR 0x1200
#define ADC_BASE_ADDR 0x1400
#define SRAM_BASE_ADDR 0x1800

#define ABS(val) ((val) > 0 ? val : (-val))

#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit)(reg & (1 << bit))

void init();

void led_toggle();

void SRAM_test(void);

#endif /*_PING_PONG_LIBRARY_*/