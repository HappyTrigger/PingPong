#include "DAC_DRIVER.h"
#include "CONTROL_DRIVER.h"


extern "C" {
#include "CONTROL_DRIVER.h"
#include "MCP_DRIVER.h"
#include "MCP_ADDRESSES.h"
#include "SPI_DRIVER.h"
#include "COM_LIB.h"
#include "SERVO_DRIVER.h"
#include "DAC_DRIVER.h"
#include "CONTROL_DRIVER.h"
}

#include <Servo.h>
#include <Wire.h>

CANMessage message, message2;
JoystickPosition j_position;
TouchpadData t_data;
Mode g_mode;
Servo myservo;
uint8_t temp;


uint8_t x;
uint8_t dir;


void setup()
{
	//dac_init();
  pinMode(A3, OUTPUT);
  pinMode(53, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A12, OUTPUT);



  digitalWrite(A3, LOW);
  digitalWrite(A5, HIGH);
  digitalWrite(A6, HIGH);
  
  
	Serial.begin(9600);
	
	//Can init
	CAN_init();

	//Servo Init
	myservo.attach(9, 900,2100);


  Wire.begin();
  x = 0;
  dir = 0;
}

void loop()
{

  if( SUCCESS == receive_and_decode_message(&j_position, &t_data, &g_mode))
  {
  
    /*Serial.print(j_position.xaxis);
    Serial.print(", ");
    Serial.print(j_position.yaxis);
    Serial.println(".");

    Serial.print(t_data.rightTouchPad);
    Serial.print(", ");
    Serial.print(t_data.leftTouchPad);
    Serial.println(".");
  
    Serial.print(t_data.rightButton);
    Serial.print(", ");
    Serial.print(t_data.leftButton);
    Serial.println(".");*/
    temp = map(j_position.xaxis, 0, 255, 0, 180);
    myservo.write(temp);
    Serial.println(temp);
    
    //Serial.println(is_IR_interrupted());

   //speed_controller(temp);
    Wire.beginTransmission(0b0101000); // transmit to device #8
    Wire.write(0);
    Wire.write(temp);              // sends one byte
    Wire.endTransmission();    // stop transmitting
    delay(1000);
    digitalWrite(A12, HIGH);
    delay(1000);
    digitalWrite(A12, LOW);
  }  
}
