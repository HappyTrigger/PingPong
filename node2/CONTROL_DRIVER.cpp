// 
// 
// 

#include "CONTROL_DRIVER.h"
#include "DAC_DRIVER.h"
#include <stdint.h>


int16_t ENCODER_MAX_VALUE = 0;
int8_t ENCODER_MIN_VALUE = 0;

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

int16_t return_max()
{
  return ENCODER_MAX_VALUE;  
}

int8_t speed_controller(int8_t speed_value)
{
  
	int8_t abs_value = speed_value;

  if (abs_value < 0)
  {//Move to the left
    digitalWrite(MJ1_DIR, DIR_LEFT);
  }
  else
  {//move to the right
    digitalWrite(MJ1_DIR, DIR_RIGHT);
    
  }
  abs_value = abs(abs_value);
  
  dac_write(abs_value);
  
  return abs_value;
  
}


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


void encoder_calibration()
{
  speed_controller(-100);
  delay(2000);
  
  
  digitalWrite(MJ1_RST, LOW);
  delay(1);
  digitalWrite(MJ1_RST, HIGH);
  ENCODER_MIN_VALUE = read_encoder();
  
  speed_controller(100);
  delay(2000);
  ENCODER_MAX_VALUE = read_encoder();
}


void position_controller(uint8_t position_value)
{
  
}













