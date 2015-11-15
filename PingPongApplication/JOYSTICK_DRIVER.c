/*
 * CFile1.c
 *
 * Created: 10.09.2015 14:41:18
 *  Author: haakongn
 */ 

#include "JOYSTICK_DRIVER.h"

#include <avr/io.h>

#include "PING_PONG_LIB.h"
#include "ADC_DRIVER.h"
#include <stdlib.h>

/****************************************************************************
* \brief Read joystick position and adjust it according to calibration data
*
* \param in calibration data
* \return Joystick position
****************************************************************************/
JoystickPosition read_joystick_position()
{
	JoystickPosition position;

	position.xaxis = ADC_read(ADC_CH2);
	//position.xaxis = ((((position.xaxis - calibration.xaxis)*100)/255)-50)*2;

	position.yaxis = ADC_read(ADC_CH1);
	//position.yaxis = ((((position.yaxis - calibration.yaxis)*100)/255)-50)*2;
	
	return position;
}

/****************************************************************************
* \brief Read joystick direction according to joystick position data
*
* \return Joystick direction
****************************************************************************/
JoystickDirection read_joystick_direction(JoystickPosition joystickPos)
{
	JoystickDirection xAxes, yAxes, retVal;
	
	if (joystickPos.xaxis > 220)
	{
		xAxes = Right;
	}
	else if (joystickPos.xaxis < 20)
	{
		xAxes = Left;
	}
	else
	{
		xAxes = Neutral;
	}
	
	if (joystickPos.yaxis > 220)
	{
		yAxes = Up;
	}
	else if (joystickPos.yaxis < 20)
	{
		yAxes = Down;
	}
	else{
		yAxes = Neutral;
	}
	
	if(abs(joystickPos.yaxis-145) > abs(joystickPos.xaxis-135))
	{
		retVal = yAxes;
	}
	else
	{
		retVal = xAxes;
	}
	
	return retVal;
}


/****************************************************************************
* \brief Calibrate joystick
*
* \return Joystick calibration data
****************************************************************************/
JoystickPosition joystick_calibration()
{
	
	JoystickPosition position;
	position.xaxis = ADC_read(ADC_CH2);
	position.yaxis = ADC_read(ADC_CH1);
	position.xaxis -= 127;
	position.yaxis -= 127;

	return position;
}

/****************************************************************************
* \brief Read Touchpad data
*
* \return Touchpad data
****************************************************************************/
TouchpadData read_touchpad_data()
{
	TouchpadData touchpad;
	touchpad.rightTouchPad = ADC_read(ADC_CH3);
	touchpad.leftTouchPad = ADC_read(ADC_CH4);
	
	if(test_bit(BUTTON_PORT, LEFT_BUTTON_PIN))
	{
		touchpad.leftButton = 1;
	}
	else
	{
		touchpad.leftButton = 0;
	}
	if(test_bit(BUTTON_PORT, RIGHT_BUTTON_PIN))
	{
		touchpad.rightButton = 1;
	}
	else
	{
		touchpad.rightButton = 0;
	}
	return touchpad;
}

/***********************************************************************************
* \brief Calculates the change in x axis position since last call of the function
*
* \return Direction of joystick
***********************************************************************************/
JoystickDirection change_xaxis(JoystickDirection direction)
{
	static JoystickDirection old_x_direction = Neutral;
	
	if(direction!= Right && direction != Left)
	{
		old_x_direction = Neutral;
		return Neutral;
	}
	
	if (old_x_direction != direction)
	{
		old_x_direction = direction;
		return direction;
	}
	
	old_x_direction = direction;
	return Neutral;
}

/***********************************************************************************
* \brief Calculates the change in y axis position since last call of the function
*
* \return Direction of joystick
***********************************************************************************/
JoystickDirection change_yaxis(JoystickDirection direction)
{
	static JoystickDirection old_y_direction = Neutral;
	
	if(direction!= Up && direction != Down)
	{
		old_y_direction = Neutral;
		return Neutral;
	}
	
	if (old_y_direction != direction)
	{
		old_y_direction = direction;
		return direction;
	}
	
	old_y_direction = direction;
	return Neutral;
}

/***********************************************************************************
* \brief Return the change in touch button
*
* \return True if the position of particular touch button changed
***********************************************************************************/
ChangeTouchpadData change_touchpad_data(TouchpadData new_touchpad_data)
{
	static TouchpadData old_touchpad_data;
	ChangeTouchpadData ret_val;
	
	ret_val.rightButton = 0;
	ret_val.leftButton = 0;
	
	if (old_touchpad_data.leftButton == 0 && new_touchpad_data.leftButton == 1)
	{
		old_touchpad_data.leftButton = 1;
		ret_val.leftButton = 1;
	}
	else if (old_touchpad_data.leftButton == 1 && new_touchpad_data.leftButton == 0)
	{
		old_touchpad_data.leftButton = 0;
	}
	
	if (old_touchpad_data.rightButton == 0 && new_touchpad_data.rightButton == 1)
	{
		old_touchpad_data.rightButton = 1;
		ret_val.rightButton = 1;
	}
	else if (old_touchpad_data.rightButton == 1 && new_touchpad_data.rightButton == 0)
	{
		old_touchpad_data.rightButton = 0;
	}
	
	return ret_val;
}