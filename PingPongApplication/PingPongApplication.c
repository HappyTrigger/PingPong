/*
 * PingPongApplication.c
 *
 * Created: 27.08.2015 15:29:02
 *  Author: haakongn
 */ 

#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdio.h>
//#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART_DRIVER.h"
#include "PING_PONG_LIB.h"
#include "JOYSTICK_DRIVER.h"
#include "OLED_DRIVER.h"




int main(void)
{	
	
	clear_bit(DDRD, PD3);
	clear_bit(DDRD, PD2);
	
	init();
	TouchpadData data; 
	int i = 0;
	JoystickPosition position;
	JoystickPosition calibration;
	calibration = joystick_calibration();
	//sei();
	
	//SRAM_test();
	
	init_oled();
	
	clear_oled();
	reset_position();
	
	
	
	print_string(" Hello World ");
	
	set_position(1, 2);
	
	print_string("You are so beautiful.... jk");
	
	
	
	while(1)
	{
		
		//refresh_oled();
		screensaver();




		
	//	position = read_joystick_position(calibration);
		
	//	printf("The x-axis is: %d  ", position.xaxis);
	//	printf("  The y-axis is: %d \n ", position.yaxis);
		
		//_delay_ms(200);
		
		
	//	data = read_touchpad_data();
		
	//	printf("The left pad is: %d  ", data.leftTouchPad);
	//	printf("  The right pad is: %d \n ", data.rightTouchPad);
		
	//	if(data.rightButton || data.leftButton)
	//	{
	//		led_toggle();
	//	}
		//SRAM_test();
		//led_toggle();
		//printf(" The value is: %d \n ", 2);
		
	}
}

