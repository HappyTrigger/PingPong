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
#include <avr/pgmspace.h>


#include "MCP_ADDRESSES.h"
#include "MCP_DRIVER.h"
#include "COM_LIB.h"




int main(void)
{	

	clear_bit(DDRD, PD3);
	clear_bit(DDRD, PD2);
	
	init();
	volatile char SPIdata = 0;
	TouchpadData data; 
	int i = 0;
	JoystickPosition position;
	JoystickPosition calibration;
	JoystickDirection direction;
	JoystickDirection change_y, change_x;
	ScreenName screen_name = 0;
	calibration = joystick_calibration();
	CANMessage canMessage, canMessage2;
	
	//SRAM_test();
	
	CAN_init();
	
	clear_oled();
	reset_position();
	
	clear_oled();
	refresh_oled();
	

	
	canMessage.ID = 0xAA;
	canMessage.length = 1;
	canMessage.data_array[0] = 0x10;
	
	canMessage2.ID = 0;
	canMessage2.length = 0;
	
	printf("start\n");
			
	/*		
			_delay_ms(1000);
			
			printf("%d\n", mcp_read(MCP_CANINTF));
	*/
	//sei();
	while(1)
	{
		//CAN_send_message(canMessage);
		//_delay_ms(200);
		//cli();
		/*i = CAN_receive_message(&canMessage2);
		
		if (i == SUCCESS)
		{
			//printf("%d, length = %d, message %d\n", canMessage2.ID, canMessage2.length, canMessage2.data_array[0]);
			canMessage2.ID = 0;
			canMessage2.length = 0;
		}
		else
		{
			printf("Nothing to receive\n");
		}*/
		
		//_delay_ms(200);
		
		//interface_state_machine(calibration);
	
		position = read_joystick_position(calibration);
		data = read_touchpad_data();
		
		printf("The x-axis is: %d  ", position.xaxis);
		printf("  The y-axis is: %d \n ", position.yaxis);
		//_delay_ms(100);
		send_joystick_possition(position, data);
		_delay_ms(1);
		//send_touchpad_possition(data);
		//send_joystick_possition(position);
		
		//printf("The left pad is: %d  ", data.leftTouchPad);
		//printf("  The right pad is: %d \n ", data.rightTouchPad);
		_delay_ms(1);
		//send_touchpad_possition(data);
		
		//_delay_ms(100);
		//send_buttons_status(data);
		
		/*if(data.rightButton || data.leftButton)
		{
			led_toggle();
		}*/

		
		//SRAM_test();
		//led_toggle();
		//printf(" The value is: %d \n ", 2);
	}
}

