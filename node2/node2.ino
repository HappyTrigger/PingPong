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


CANMessage message, message_node1;
JoystickPosition j_position;
TouchpadData t_data;
Mode g_mode;
Servo myservo;
uint16_t temp;
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
  encoder_calibration();
  temp_bit = return_max();
  Serial.println(temp_bit);
  delay(1000);
  speed_controller(0,2); 

  //Shooting-mech
  pinMode(SHOOTING_PIN, OUTPUT);
  digitalWrite(SHOOTING_PIN, HIGH);


  g_mode.gamemode = 0;
  
  
}

void loop()
{

  if( SUCCESS == receive_and_decode_message(&j_position, &t_data, &g_mode))
  {

   //ServoController
    myservo.write(map(j_position.xaxis, 0, 255, 180, 0));
    //Serial.println(temp);
   //Serial.println(change_touchpad_data(t_data.rightButton));
   
   
   //Shooting_mech
   if(change_touchpad_data(t_data.rightButton) == 1)
   {
      digitalWrite(SHOOTING_PIN, LOW);
      delay(90);
      digitalWrite(SHOOTING_PIN, HIGH);
   }
    //MotorSpeed
    //temp = map(j_position.xaxis, 0, 255, -100, 100);
    //temp = speed_controller(temp);
    //Serial.println(temp);
  }  
 
  
  //Position-controller
  temp = position_controller(t_data.rightTouchPad);


  if(is_IR_interrupted()){
    //Something is blocking the IR
    //Do nothing if not playing the game
    if(g_mode.gamemode >= 1 && g_mode.gamemode <= 5 ){
      //Gamelost 
        g_mode.gamemode = Endgame;
    }
  }

  switch(g_mode.gamemode){
    case Tutorial:
		Serial.println(1);
		break;
    case Easy:
		Serial.println(2);
		break;
    case Normal:
		Serial.println(3);
		break;
    case Hard:
		Serial.println(4);
		break;
    case Insane:
		Serial.println(5);
		break;
    case Normal_settings:
		Serial.println(6);
		break;
    case Endgame:
		send_game_mode(Endgame);
		Serial.println(7);
		break;
    default:
		Serial.println(8);
		break;
  }
  
}




