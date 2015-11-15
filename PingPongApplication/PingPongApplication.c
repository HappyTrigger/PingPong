/*
 * PingPongApplication.c
 *
 * Created: 27.08.2015 15:29:02
 *  Author: haakongn
 */ 

#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART_DRIVER.h"
//#include "PING_PONG_LIB.h"
//#include "JOYSTICK_DRIVER.h"
//#include "OLED_DRIVER.h"
//#include "COM_LIB.h"
#include "USER_INTERFACE.h"
//#include "SPI_DRIVER.h"
//#include "CAN_DRIVER.h"
#include "EEPROM.h"
#include <avr/pgmspace.h>
#include <string.h>


//#include "MCP_ADDRESSES.h"
//#include "MCP_DRIVER.h"
//#include "TIME_DRIVER.h"


int main(void)
{	
	//int i;

	clear_bit(DDRD, PD3);
	clear_bit(DDRD, PD2);
	
	init();
	high_score scores;
	
	timer1_init();
	
	CAN_init();
	
	clear_oled();
	reset_position();
	refresh_oled();
	
	printf("start\n");
	printf("%d\n", sizeof(int));
	
	
	strncpy ( scores.username[0], "KURVA\0", 6 );
	strncpy ( scores.username[1], "KURVA\0", 6 );
	strncpy ( scores.username[2], "KURVA\0", 6 );
	strncpy ( scores.username[3], "KURVA\0", 6 );
	strncpy ( scores.username[4], "KURVA\0", 6 );
	scores.score[0] = 0x0000;
	scores.score[1] = 0x0000;
	scores.score[2] = 0x0000;
	scores.score[3] = 0x0000;
	scores.score[4] = 0x0000;
	
	high_score_write(scores);
	
	sei();
	while(1)
	{
		interface_state_machine();
		
		/*score_read = high_score_read();
		
		for (i = 0; i < 5; i++)
		{
			printf("#%d. %s : %x\n", i, score_read.username[i], score_read.score[i]);
		}*/
	}
}

