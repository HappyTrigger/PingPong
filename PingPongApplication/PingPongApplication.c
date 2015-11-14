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
#include "PING_PONG_LIB.h"
#include "JOYSTICK_DRIVER.h"
#include "OLED_DRIVER.h"
#include "USER_INTERFACE.h"
#include "SPI_DRIVER.h"
#include "CAN_DRIVER.h"
#include "EEPROM.h"
#include <avr/pgmspace.h>
#include <string.h>


#include "MCP_ADDRESSES.h"
#include "MCP_DRIVER.h"
#include "COM_LIB.h"





int main(void)
{	

	clear_bit(DDRD, PD3);
	clear_bit(DDRD, PD2);
	
	init();
	TouchpadData data; 
	int i = 0;
	JoystickPosition position;
//	JoystickDirection direction;
//	JoystickDirection change_y, change_x;
//	ScreenName screen_name = 0;
	CANMessage canMessage;
	high_score scores, score_read;
	
	

	//led_toggle();
	//SRAM_test();
	
	CAN_init();
	
	clear_oled();
	reset_position();
	
	clear_oled();
	refresh_oled();
	

	
	canMessage.ID = 0xAA;
	canMessage.length = 1;
	canMessage.data_array[0] = 0x10;
	
	
	printf("start\n");
	
	/*
	strncpy ( scores.username[0], "AAAA\0", 5 );
	strncpy ( scores.username[1], "BBBB\0", 5 );
	strncpy ( scores.username[2], "asfg\0", 5 );
	strncpy ( scores.username[3], "dfjd\0", 5 );
	strncpy ( scores.username[4], "REFW\0", 5 );
	scores.score[0] = 32;
	scores.score[1] = 22;
	scores.score[2] = 12;
	scores.score[3] = 8;
	scores.score[4] = 3;
	
	high_score_write(scores);
	*/
	//sei();
	while(1)
	{
		interface_state_machine();
	
		
		//score_read = high_score_read();
		
		
		
		//for (i = 0; i < 5; i++)
		//{
			//printf("#%d. %s : %d\n", i, score_read.username[i], score_read.score[i]);
		//}
		
	}
}

