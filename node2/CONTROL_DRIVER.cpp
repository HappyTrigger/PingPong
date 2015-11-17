#include "CONTROL_DRIVER.h"
#include "DAC_DRIVER.h"
#include <stdint.h>

/****************************************************************************
* \brief Variables used in the encoder and Speed-Controller
*
****************************************************************************/
int16_t ENCODER_MAX_VALUE = 0;
int16_t ENCODER_MIN_VALUE = 0;
uint8_t MAX_SPEED = 100;

/****************************************************************************
* \brief Variables used in the PID Controller
*
****************************************************************************/
float kp = 1.2;
float ki = 0.9;
float kd = 0.1;

double timeChange = 0.01;
uint16_t output = 0;
double errorSum, lastError;
int8_t acceptedError = 15;

/****************************************************************************
* \brief Initialize communication with motor controlling unit
*
****************************************************************************/
void control_init()
{
	pinMode(MJ1_DIR, OUTPUT);
	pinMode(53, OUTPUT);
	pinMode(MJ1_SEL, OUTPUT);
	pinMode(MJ1_RST, OUTPUT);
	pinMode(MJ1_EN, OUTPUT);
	pinMode(MJ1_OE, OUTPUT);
	
	digitalWrite(MJ1_EN, HIGH);
	digitalWrite(MJ1_SEL, HIGH);
	digitalWrite(MJ1_RST, HIGH);

	MJ2_DDR = 0x00;
}

/****************************************************************************
* \brief Return maximum value if the motor position
*
* \return Maximum value if the motor position
****************************************************************************/
int16_t return_max()
{
  return ENCODER_MAX_VALUE;  
}

/****************************************************************************
* \brief Write speed and direction to the motor
*
* \return Speed
****************************************************************************/
void speed_controller(uint16_t speed_value, uint8_t motor_direction)
{
	uint16_t abs_value = speed_value;
 
	if (motor_direction == LEFT_DIRECTION)
	{//Move to the left
	  digitalWrite(MJ1_DIR, DIR_LEFT);
	}
	else if(motor_direction == RIGHT_DIRECTION)
	{//move to the right
	  digitalWrite(MJ1_DIR, DIR_RIGHT);
	}
	abs_value = abs(abs_value);

	if(abs_value>MAX_SPEED)
	{
		abs_value = MAX_SPEED;
	}
	
	dac_write(abs_value);
}

/****************************************************************************
* \brief Read the position of the motor
*
* \return position of the motor
****************************************************************************/
int16_t read_encoder()
{
	volatile uint8_t encoder_data1;
	volatile uint8_t encoder_data2;
	volatile int16_t encoder_data;
	digitalWrite(MJ1_OE, LOW);
	digitalWrite(MJ1_SEL, LOW);
	delayMicroseconds(20);

	encoder_data1 = MJ2_PIN;
	encoder_data1 = reverse_byte_input(encoder_data1);

	digitalWrite(MJ1_SEL, HIGH);
	delayMicroseconds(20);
	
	encoder_data2 = MJ2_PIN;
	encoder_data2 = reverse_byte_input(encoder_data2);
	
	digitalWrite(MJ1_OE, HIGH);

	encoder_data = (encoder_data1 << 8) |  encoder_data2;
	return encoder_data;
}

/****************************************************************************
* \brief Reverse bit order in byte
*
* \return reversed byte
****************************************************************************/
uint8_t reverse_byte_input(uint8_t input)
{
	uint8_t output = 0;
	uint8_t tmp = 0;
	int i;

	for(i = 0; i < 8; i++)
	{
		tmp = input & 0x1;
		input >>= 1;
		output <<= 1;
		output |= tmp;
		tmp = 0;
	}  
	
	return output;
}

/****************************************************************************
* \brief Calibrate the motor
*
****************************************************************************/
void encoder_calibration()
{
	speed_controller(MAX_SPEED, LEFT_DIRECTION);
	delay(1500);
	
	digitalWrite(MJ1_RST, LOW);
	delay(1);
	digitalWrite(MJ1_RST, HIGH);
	ENCODER_MIN_VALUE = read_encoder();
	
	speed_controller(MAX_SPEED, RIGHT_DIRECTION);
	delay(1500);
	ENCODER_MAX_VALUE = read_encoder();
	while(position_controller(127));
}

/****************************************************************************
* \brief PI controller
*
* \param in desired position
****************************************************************************/
int position_controller(uint8_t position_value)
{
  static uint8_t motor_direction;
  int16_t input = read_encoder();
  input = map(input, ENCODER_MIN_VALUE, ENCODER_MAX_VALUE , 0, 255);

  double error = (position_value - input); 
  if(error < 0)
  {
    motor_direction = LEFT_DIRECTION;
  }
  else
  {
    motor_direction = RIGHT_DIRECTION;
  }
 
  error = abs(error);
  if(error < acceptedError)
  {
    speed_controller(0, motor_direction);
    errorSum = 0;
	return 0;
  }
  //int16_t dError = (error - lastError)/timeChange;
  errorSum += (error*timeChange);

  output = abs(kp*error + ki*errorSum); //kd*dError);

  lastError = error;
  //lastTime = CurrentTime; 

  speed_controller(output, motor_direction);
  return 1;
}

/****************************************************************************
* \brief PI controller, controlled only by joystick
*
* \param in desired position
* \param in current settings of controller
****************************************************************************/
int8_t joystick_position_controller(JoystickPosition joystick_position, Control_settings settings)
{
	int16_t temp = joystick_position.xaxis;
	int8_t dir;
	temp = temp - 137; // A little calibration
	if (temp < 0 )
	{
		dir = LEFT_DIRECTION;
		
		if(settings == REVERSE_SETTINGS)
		{
			dir = RIGHT_DIRECTION;
		}
	}
	else if(temp > 0)
	{//move to the right
		dir = RIGHT_DIRECTION;
		if(settings == REVERSE_SETTINGS)
		{
			dir = LEFT_DIRECTION;
		}
	}
	
	temp = abs(temp); 
	speed_controller(temp, dir);
	return temp;
}

/****************************************************************************
* \brief Change parameters of PI controller
*
* \param in proportional value
* \param in integral value
****************************************************************************/
void change_pi_param(float pi_kp, float pi_ki)
{
   kp = pi_kp;
   ki = pi_ki; 
}

/****************************************************************************
* \brief Map desired servo value depending on current settings
*
* \param in desired servo value
* \param in current settings
****************************************************************************/
uint8_t servo_value_mapping(uint8_t servo_value, Control_settings settings )
{
	uint8_t retVal;
	if(settings == REVERSE_SETTINGS)
	{
		retVal = (map(servo_value, 0, 255, 0, 180));
	}
	else
	{
		retVal = (map(servo_value, 0, 255, 180, 0));
	}
	return retVal;
}

/****************************************************************************
* \brief Map desired motor value depending on current settings
*
* \param in desired motor value
* \param in current settings
****************************************************************************/
uint8_t motor_value_mapping(uint8_t touchpad_value, Control_settings settings ){
	uint8_t retVal;
	if(settings == REVERSE_SETTINGS)
	{
		retVal = (map(touchpad_value, 0, 255, 255, 0));
	}
	else
	{
		retVal = (map(touchpad_value, 0, 255, 0, 255));
	}
	return retVal;
}

/****************************************************************************
* \brief Change accepted error of PI controller
*
* \param in new value of accepted error
****************************************************************************/
void change_accepted_error(uint8_t error)
{
	acceptedError = error;
}

/****************************************************************************
* \brief Change maximal speed of motor
*
* \param in new maximal speed
****************************************************************************/
void change_max_speed(uint8_t max_speed)
{
	MAX_SPEED = max_speed;
}


/****************************************************************************
* \brief Degrease the speed depending on old and current measured values
*
* \param in new value of speed
****************************************************************************/
uint8_t speed_decrease(uint8_t new_speed)
{
	static uint8_t speed_dif = 0;
	static uint8_t old_measured_speed = 100;
	uint8_t dif = old_measured_speed > new_speed ? old_measured_speed - new_speed : new_speed - old_measured_speed;
	
	if (dif > 4)
	{
		speed_dif = speed_dif < old_measured_speed ? speed_dif + 15: speed_dif;
	}
	else
	{
		old_measured_speed = new_speed;
		speed_dif = 0;
	}
	
	return speed_dif;
}

