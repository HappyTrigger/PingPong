
extern "C" {
#include "CAN_DRIVER.h"
#include "MCP_DRIVER.h"
#include "MCP_ADDRESSES.h"
#include "SPI_DRIVER.h"
#include "COM_LIB.h"
}

CANMessage message, message2;
JoystickPosition j_position;

void setup()
{
	pinMode(53, OUTPUT);
  Serial.begin(9600);
	CAN_init();
	message.ID = 0x12;
	message.length = 1;
	message.data_array[0] = 0x92;
}

void loop()
{
  ///message2.ID = 0x00;
  //message2.length = 0;
  //message2.data_array[0] = 0x00;
  
	CAN_send_message(message);
  //Serial.print(message.ID);
  //Serial.print(", ");
  //Serial.print(message.length);
  //Serial.print(", ");
  //Serial.print(message.data_array[0]);
  //Serial.println(".");

  if( SUCCESS == receive_joistick_position(&j_position))
  {
    Joystick_pwm(&j_position);
    Serial.print(j_position.xaxis);
    Serial.print(", ");
    Serial.print(j_position.yaxis);
    Serial.println(".");
   
  }

  //CAN_receive_message(&message2);
  
}
