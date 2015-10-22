

extern "C" {
#include "CAN_DRIVER.h"
#include "MCP_DRIVER.h"
#include "MCP_ADDRESSES.h"
#include "SPI_DRIVER.h"
#include "COM_LIB.h"
#include "SERVO_DRIVER.h"
}

#include <Servo.h>

CANMessage message, message2;
JoystickPosition j_position;
TouchpadData t_data;
Servo myservo;
uint8_t temp;


void setup()
{
	pinMode(53, OUTPUT);
	Serial.begin(9600);
	CAN_init();
	message.ID = 0x12;
	message.length = 1;
	message.data_array[0] = 0x92;
	myservo.attach(9, 900,2100);

}

void loop()
{
  ///message2.ID = 0x00;
  //message2.length = 0;
  //message2.data_array[0] = 0x00;
  
	//CAN_send_message(message);
  //Serial.print(message.ID);
  //Serial.print(", ");
  //Serial.print(message.length);
  //Serial.print(", ");
  //Serial.print(message.data_array[0]);
  //Serial.println(".");



  if( SUCCESS == receive_and_decode_message(&j_position, &t_data))
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
    Serial.println(j_position.xaxis);
    temp = map(j_position.xaxis, 0, 255, 0, 180);
    Serial.println(temp);
    //Serial.println(t_data.rightTouchPad);
    //Serial.println(t_data.leftTouchPad);
    myservo.write(temp);
    //Serial.println(is_IR_interrupted());
  }

  //CAN_receive_message(&message2);
  
}
