/*
 * CFile1.c
 *
 * Created: 10.09.2015 16:17:18
 *  Author: haakongn
 */ 
#include "ADC_DRIVER.h"

#include <avr/delay.h>

#include "PING_PONG_LIB.h"

/****************************************************************************
* \brief Read data from ADC
*
****************************************************************************/
int read_adc(char channel)
{
	volatile char *ext_adc = (char *) ADC_BASE_ADDR;
	
	*ext_adc = channel;
	_delay_ms(1);
	return *ext_adc;
}