// 
// 
// 

#include "CONTROL_DRIVER.h"
#include "DAC_DRIVER.h"
#include <stdint.h>



/****************************************************************************
* \brief Variables used in the encoder and Speed-Controller
*
****************************************************************************/
int16_t ENCODER_MAX_VALUE = 0;
int16_t ENCODER_MIN_VALUE = 0;
uint8_t MAX_SPEED = 125;

/****************************************************************************
* \brief Variables used in the PID Controller
*
****************************************************************************/
float kp = 1.4;
float ki = 1;
float kd = 0.1;

double timeChange = 0.01;
uint16_t output = 0;
double errorSum, lastError;
int8_t acceptedError = 5;

const int RIGHT_DIRECTION = 2;
const int LEFT_DIRECTION = 1;



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

	if(abs_value>MAX_SPEED){
		abs_value = MAX_SPEED;
	}
	
	dac_write(abs_value);
	
	//return abs_value;
  
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
}


int position_controller(uint8_t position_value)
{
  static uint8_t motor_direction;
  int16_t input = read_encoder();
  input=map(input, ENCODER_MIN_VALUE, ENCODER_MAX_VALUE , 0, 255);

 // unsigned long CurrentTime = millis();
  //double timeChange = (double) CurrentTime - lastTime;
  

  double error = (position_value - input); 
  if(error<0)
  {
    motor_direction = LEFT_DIRECTION;
  }
  else{
    motor_direction = RIGHT_DIRECTION;
  }
 
  error = abs(error);
  if(error < acceptedError){
    speed_controller(0, motor_direction);
    errorSum=0;
  }
  int16_t dError = (error - lastError)/timeChange;
  errorSum += (error*timeChange);

  output = abs(kp*error + ki*errorSum); //kd*dError);


 
  lastError = error;
  //lastTime = CurrentTime; 


  speed_controller(output, motor_direction);
  return output;
}













