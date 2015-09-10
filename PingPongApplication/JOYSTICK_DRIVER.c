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