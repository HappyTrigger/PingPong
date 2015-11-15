#include "DAC_DRIVER.h"
#include "CONTROL_DRIVER.h"
#include "COM_LIB.h"

#include "MCP_DRIVER.h"
#include "MCP_ADDRESSES.h"
#include "SPI_DRIVER.h"
#include "IR_DRIVER.h"


#include <Servo.h>
#include <Wire.h>


#define SHOOTING_PIN 7


CANMessage message, message_node1;
JoystickPosition j_position;
TouchpadData t_data;
Mode g_mode;
Servo myservo;
Control_settings g_setting;



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

  //GameMode init
  g_mode.gamemode = No_mode;
  g_setting = NORMAL_SETTINGS;
  
  


}

void loop()
{
	int i;

  if( SUCCESS == receive_and_decode_message(&j_position, &t_data, &g_mode))
  {
   //ServoController
   //Serial.println(j_position.xaxis);
   //servo_value_mapping(j_position.xaxis, g_setting);
   

   
   if(g_setting == NORMAL_SETTINGS){
	   myservo.write(map(j_position.xaxis, 0, 255, 180, 0));
   }
   else if(g_setting == REVERSE_SETTINGS){
	   myservo.write(map(j_position.xaxis, 0, 255, 0, 180));
   }
	
   //Shooting_mech
   /*
   if(change_touchpad_data(t_data.rightButton) == 1)
   {
      digitalWrite(SHOOTING_PIN, LOW);
      delay(50);
      digitalWrite(SHOOTING_PIN, HIGH);
   }*/
   
  }  

/*
  if(is_IR_interrupted()){
    //Something is blocking the IR
    //Do nothing if not playing the game
    if(g_mode.gamemode >= 1 && g_mode.gamemode <= 5 && g_mode.gamemode == Reverse_settings || g_mode.gamemode == Normal_settings ){
      //Gamelost 
        g_mode.gamemode = Endgame;
    }
  }*/

  if(change_touchpad_data(t_data.leftButton) == 1){
	  g_mode.gamemode = Endgame;
  }


  switch(g_mode.gamemode){
    case No_mode:
		break;
	
	case Tutorial:
		temp = position_controller(t_data.rightTouchPad);
		break;
		
    case Easy:
		temp_bit=joystick_position_controller(&j_position);
		
		break;
		
    case Normal:
		temp = position_controller(t_data.rightTouchPad);
		break;
		
    case Hard:
		temp = position_controller(t_data.rightTouchPad);
		break;
		
    case Insane:
		temp = position_controller(t_data.rightTouchPad);	
		change_pi_param(1.3, 2.5);
		break;
		
    case Settings_normal:
		g_setting = NORMAL_SETTINGS;
		change_pi_param(1.7, 0.9);
		break;
		
    case Settings_reverse:
		g_setting = REVERSE_SETTINGS;
		break;
		
    case Endgame:
		end_game();
		g_mode.gamemode=0;
		g_setting = NORMAL_SETTINGS;
		break;
		
    default:
		//Serial.println(9);
		break;
  }
  
}




