// 
// 
// 

#include "CONTROL_DRIVER.h"
#include <stdint.h>
#include "arduino.h"




char speed_controller( speed_value)
{
  
	char abs_value = speed_value;

  if (abs_value < 83)
  {//Move to the left
    digitalWrite(A3, HIGH);
  }
  else
  {//move to the right
    digitalWrite(A3, LOW);
    
  }
  if(abs_value > 130 || abs_value < 30)
  {
    abs_value = 100;
  }
  else
  {
    abs_value = 0;
  }

  return abs_value; 
  
}
