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

uint16_t random_event_timer;
uint16_t speed_decrease_time;
uint16_t time;
uint8_t speed_dec = 0;

void setup()
{
	control_init();
	
	//Serial init
	Serial.begin(9600);
	
	//Can init
	CAN_init();

	//Servo Init
	myservo.attach(9, 900,2100);
  
	//DAC init
	Wire.begin();
	
	//Calibration of game 
	encoder_calibration();
	
	//Shooting-mech initialization
	pinMode(SHOOTING_PIN, OUTPUT);
	digitalWrite(SHOOTING_PIN, HIGH);

	//value init
	g_mode.gamemode = No_mode;
	g_setting = NORMAL_SETTINGS;
	j_position.xaxis = 127;
	j_position.yaxis = 127;
	t_data.rightTouchPad = 127;
	t_data.leftTouchPad = 127;
  
	//initialize random
	randomSeed(analogRead(0));
	speed_decrease_time = random_event_timer = millis() / 1000;
	random_event_timer += 20;
	speed_decrease_time += 2;
}

void loop()
{
	time = millis()/1000;
	if( SUCCESS == receive_and_decode_message(&j_position, &t_data, &g_mode))
	{
		//Shooting_mech
		if(change_touchpad_data(t_data.rightButton) == 1)
		{
		  digitalWrite(SHOOTING_PIN, LOW);
		  delay(50);
		  digitalWrite(SHOOTING_PIN, HIGH);
		}
		//Serial.println(change_touchpad_data(t_data.rightButton));
	}  

	if(is_IR_interrupted())
	{
		//Something is blocking the IR
		//Do nothing if not playing the game
		if(g_mode.gamemode >= Tutorial && g_mode.gamemode <= Insane)
		{
			//Game over 
			g_mode.gamemode = Endgame;
		}
	}

	switch(g_mode.gamemode)
	{
		case No_mode:
			break;
		
		case Tutorial:
			position_controller(motor_value_mapping(t_data.rightTouchPad, g_setting));
			myservo.write(servo_value_mapping(j_position.xaxis, g_setting));
			break;
			
	  case Easy:
			joystick_position_controller(j_position, g_setting);
			myservo.write(servo_value_mapping(j_position.yaxis, g_setting));	
			break;
			
	  case Normal:
			position_controller(motor_value_mapping(t_data.rightTouchPad, g_setting));
			myservo.write(servo_value_mapping(j_position.xaxis, g_setting));
			break;
			
	  case Hard:
			uint8_t hard_value, temp_data;
			static uint8_t old_hard_value;
			
			temp_data = t_data.leftTouchPad;
			hard_value = temp_data;
			hard_value = map(hard_value, 0, 255, 0, 130);
			
			if (speed_decrease_time < time)
			{
				old_hard_value = hard_value;
				speed_dec = speed_decrease(hard_value);
				speed_decrease_time = time + 1;
				//Serial.println(speed_decrease(150));
			}
			if (t_data.leftTouchPad != temp_data)
			{
				//old_hard_value = hard_value;
				//speed_dec = speed_decrease(hard_value);
				speed_dec = 0;
			}
			speed_dec = speed_dec > hard_value ? 0 : speed_dec;
			change_max_speed(hard_value - speed_dec);
			
			change_accepted_error(3);
			position_controller(motor_value_mapping(t_data.rightTouchPad, g_setting));
			myservo.write(servo_value_mapping(j_position.xaxis, g_setting));
			change_pi_param(3.3, 2.5);
			break;
			
	  case Insane:
			static uint8_t insane_value = 0;
			if(random_event_timer < time)
			{
				request_minigame();
				random_event_timer = time + 20;
			}
			insane_value = map(random(0,255), 0, 255, 0, 150);
			change_max_speed(120);
			change_accepted_error(3);
			change_pi_param(3.3, 2.5);
			
			position_controller(motor_value_mapping(t_data.rightTouchPad, g_setting));
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




