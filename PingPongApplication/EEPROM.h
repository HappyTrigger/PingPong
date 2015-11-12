/*
 * EEPROM.h
 *
 * Created: 12.11.2015 12:15:20
 *  Author: haakongn
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

void EEPROM_write(uint16_t uiAddress, uint8_t ucData);
uint8_t EEPROM_read(uint16_t uiAddress);


#endif /* EEPROM_H_ */