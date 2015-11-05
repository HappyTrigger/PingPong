#include "DAC_DRIVER.h"
#include "CONTROL_DRIVER.h"
extern "C" {
#include "MCP_DRIVER.h"
#include "MCP_ADDRESSES.h"
#include "SPI_DRIVER.h"
#include "COM_LIB.h"
#include "SERVO_DRIVER.h"
}

#include <Servo.h>
#include <Wire.h>


#define SHOOTING_PIN 7


CANMessage message, message2;
JoystickPosition j_position;
TouchpadData t_data;
Mode g_mode;
Servo myservo;
int8_t temp;
int16_t temp_bit;







void setup()
{
	control_init();

  
  //Serial printing
	Serial.begin(9600);
	
	//Can init
	CAN_init();

	//Servo Init
	myservo.attach(9, 900,2100);
  
  //DAC init
  dac_init();
  //encoder_calibration();
  temp_bit = return_max();
  Serial.println(temp_bit);
  delay(1000);

  //Shooting-mech
 // pinMode(SHOOTING_PIN, OUTPUT);


  
  
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
   
   
   //ServoController
   // temp = map(j_position.xaxis, 0, 255, 0, 180);
   //myservo.write(temp);
    //Serial.println(temp);
   Serial.println(t_data.rightButton);
   //Shooting_mech
   if(t_data.rightButton == 1){
     // digitalWrite(SHOOTING_PIN, LOW);
    //  delay(100);
    //  digitalWrite(SHOOTING_PIN, HIGH);
   }

   
    //MotorSpeed
    temp = map(j_position.xaxis, 0, 255, -100, 100);
    temp=speed_controller(temp);
    //Serial.println(temp);
    //dac_write(temp);

    
    temp_bit = read_encoder();
    //Serial.println(temp_bit);
    
    
    //Serial.println(is_IR_interrupted());
 
  }  
  
}




