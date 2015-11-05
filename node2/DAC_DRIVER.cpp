#include <Wire.h>

// 
// 
// 

#include "DAC_DRIVER.h"


void dac_init()
{
  Wire.begin();
}

void dac_write(int8_t value)
{
    Wire.beginTransmission(0b0101000); // transmit to device #8
    Wire.write(0);                   //Command 
    Wire.write(value);              // sends one byte
    Wire.endTransmission();    // stop transmitting
}
