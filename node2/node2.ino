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
long random_event_timer;





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
  delay(1000);
  speed_controller(0,LEFT_DIRECTION); 

  //Shooting-mech
  pinMode(SHOOTING_PIN, OUTPUT);
  digitalWrite(SHOOTING_PIN, HIGH);

  //value init
  g_mode.gamemode = No_mode;
  g_setting = NORMAL_SETTINGS;
  j_position.xaxis = 127;
  j_position.yaxis = 127;
  t_data.rightTouchPad = 127;
  t_data.leftTouchPad = 127;
  
  
  randomSeed(analogRead(0));
}

void loop()
{
	int i;

  if( SUCCESS == receive_and_decode_message(&j_position, &t_data, &g_mode))
  {
   //Shooting_mech
   if(change_touchpad_data(t_data.rightButton) == 1)
   {
      digitalWrite(SHOOTING_PIN, LOW);
      delay(50);
      digitalWrite(SHOOTING_PIN, HIGH);
   }
  }  


  if(is_IR_interrupted())
  {
    //Something is blocking the IR
    //Do nothing if not playing the game
    if(g_mode.gamemode >= Tutorial && g_mode.gamemode <= Insane)
	{
      //Gamelost 
        g_mode.gamemode = Endgame;
    }
  }

  switch(g_mode.gamemode){
    case No_mode:
		break;
	
	case Tutorial:
		position_controller(motor_value_mapping(t_data.rightTouchPad, g_setting));
		myservo.write(servo_value_mapping(j_position.xaxis, g_setting));
		break;
		
    case Easy:
		joystick_position_controller(&j_position, g_setting);
		myservo.write(servo_value_mapping(j_position.yaxis, g_setting));	
		break;
		
    case Normal:
		position_controller(motor_value_mapping(t_data.rightTouchPad, g_setting));
		myservo.write(servo_value_mapping(j_position.xaxis, g_setting));
		break;
		
    case Hard:
		static uint8_t hard_value;
		hard_value = map(t_data.leftTouchPad, 0, 255, 0, 150);
		change_max_speed(hard_value);
		change_accepted_error(3);
		temp = position_controller(motor_value_mapping(t_data.rightTouchPad, g_setting));
		myservo.write(servo_value_mapping(j_position.xaxis, g_setting));
		change_pi_param(3.3, 2.5);
		break;
		
    case Insane:
		static uint8_t insane_value = 0;
		random_event_timer = (millis() / 1000) % 20;
		Serial.println(random_event_timer);
		if(random_event_timer > 19)
		{
			request_minigame();
		}
		insane_value = map(random(0,255), 0, 255, 0, 150);
		change_max_speed(120);
		change_accepted_error(3);
		change_pi_param(3.3, 2.5);
		
		temp = position_controller(motor_value_mapping(t_data.rightTouchPad, g_setting));
		myservo.write(servo_value_mapping(j_position.xaxis, g_setting));
		break;
		
    case Settings_normal:
		g_setting = NORMAL_SETTINGS;
		change_pi_param(1.7, 0.9);
		change_accepted_error(15);
		change_max_speed(100);
		break;
		
    case Settings_reverse:
		g_setting = REVERSE_SETTINGS;
		change_pi_param(1.7, 0.9);
		change_accepted_error(15);
		change_max_speed(100);
		break;
		
    case Endgame:
		end_game();
		myservo.write(servo_value_mapping(125, g_setting));
		while(position_controller(127));
		g_mode.gamemode = No_mode;
		change_max_speed(100);
		change_accepted_error(15);
		break;
		
    default:
		break;
  }
  
}




