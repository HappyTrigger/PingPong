/*
 * CFile1.c
 *
 * Created: 10.09.2015 14:41:18
 *  Author: haakongn
 */ 
#include "JOYSTICK_DRIVER.h"
#include "PING_PONG_LIB.h"
#include "ADC_DRIVER.h"

JoystickPosition read_joystick_position(JoystickPosition calibration)
{
	JoystickPosition position;

	position.xaxis = read_adc(ADC_CH2);
	position.xaxis = ((((position.xaxis - calibration.xaxis)*100)/255)-50)*2;

	position.yaxis = read_adc(ADC_CH1);
	position.yaxis = ((((position.yaxis - calibration.yaxis)*100)/255)-50)*2;
	
	return position;
}


JoystickDirection read_joystick_direction(JoystickPosition joystickPos)
{
	JoystickDirection xAxes, yAxes, retVal;
	
	if (joystickPos.xaxis > 5)
	{
		xAxes = Right;
	}
	else if (joystickPos.xaxis < -5)
	{
		xAxes = Left;
	}
	else
	{
		xAxes = Neutral;
	}
	
	if (joystickPos.yaxis > 5)
	{
		yAxes = Up;
	}
	else if (joystickPos.yaxis < -5)
	{
		yAxes = Down;
	}
	else{
		yAxes = Neutral; 
	}
	
	if(ABS(joystickPos.yaxis) > ABS(joystickPos.xaxis))
	{
		retVal = yAxes;
	}
	else
	{
		retVal = xAxes;
	}
	
	return retVal;
}



JoystickPosition joystick_calibration(){
	
	JoystickPosition position;
	int i = 0;
	position.xaxis = read_adc(ADC_CH2);
	position.yaxis = read_adc(ADC_CH1);
	position.xaxis -= 127;
	position.yaxis -= 127;

	
	//for (i = 0; i < 100; i++)
	//{
		//position.xaxis += read_adc(ADC_CH2);
		//position.yaxis += read_adc(ADC_CH1);
		//_delay_ms(2);
	//}
	//position.xaxis /= 100;
	//position.yaxis /= 100;
	//position.xaxis -= 127;
	//position.yaxis -= 127;
	//
	return position;
	
	
}

#define ADC_CH1 0b00000100 // X axis
#define ADC_CH2 0b00000101 // Y axis
#define ADC_CH3 0b00000110 // Slider right
#define ADC_CH4 0b00000111 // Slider left

TouchpadData read_touchpad_data(){
	TouchpadData touchpad;
	touchpad.rightTouchPad = read_adc(ADC_CH3);
	touchpad.leftTouchPad = read_adc(ADC_CH4);
	
	if(test_bit(PIND, PD3)){
		touchpad.leftButton = 1;
		printf("Left button pressed \n");
	}else{
		touchpad.leftButton = 0;
	}
	if(test_bit(PIND, PD2)){
		touchpad.rightButton = 1;
		printf("Right button pressed \n");
	}else{
		touchpad.rightButton = 0;
		
	}
	return touchpad;
}