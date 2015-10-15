/*
 * CFile1.c
 *
 * Created: 04.09.2015 15:35:58
 *  Author: michalma
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "PING_PONG_LIB.h"
#include "OLED_DRIVER.h"
#include "USART_DRIVER.h"
#include "USER_INTERFACE.h"



/****************************************************************************
* \brief Initialize all important peripherals and pins
*
****************************************************************************/
void init()
{
	USART_init(MYUBBR);
	/* Enables standard IO functions */	
	fdevopen(USART_putchar, USART_getchar);
	/* Enables external memory*/
	set_bit(MCUCR, SRE);
	/* Pin 1 is set to output for the LED*/
	set_bit(DDRB, PB0);
	/* Pin is now driven LOW*/
	clear_bit(PORTB, PB0);
	/* Initialize OLED display */
	init_oled();
	interface_init();
}

/****************************************************************************
* \brief Toggles the led connected to pin 1 (PORTB, PB0)
*
****************************************************************************/
void led_toggle()
{	
	if (test_bit(PINB, PB0))
	{
		clear_bit(PORTB, PB0);
	}
	else
	{
		set_bit(PORTB, PB0);
	}
	_delay_ms(BLINK_TIME);
}


/****************************************************************************
* \brief Test function for the SRAM and Gal chip
*
****************************************************************************/
void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800;	//	Start address for the SRAM
	
	unsigned int i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	unsigned char testvalue;
	printf("Starting SRAM test...\r\n");
	
	
	for (i = 0; i < 0x400; i++)
	{
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue)
		{
			printf("SRAM error (write phase): ext_ram[%d] = %02X (should be	%02X)\r\n", i, ext_ram[i], testvalue);
			werrors++;
		}
	}
	
	for (i = 0; i < 0x400; i++)
	{
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue)
		{
			printf("SRAM error (read phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			rerrors++;
		}
	}
	printf("SRAM test completed with %d errors in write phase and %d errors in read phase\r\n", werrors, rerrors);
}


