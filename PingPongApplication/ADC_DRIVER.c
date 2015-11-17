/*
 * CFile1.c
 *
 * Created: 10.09.2015 16:17:18
 *  Author: haakongn
 */ 
#include "ADC_DRIVER.h"
#include "PING_PONG_LIB.h"
#include <avr/delay.h>

/****************************************************************************
* \brief Read data from ADC
*
****************************************************************************/
uint8_t ADC_read(uint8_t channel)
{
	volatile uint8_t* ext_adc = (uint8_t*) ADC_BASE_ADDR;
	
	*ext_adc = channel;
	_delay_ms(1);
	return *ext_adc;
}