/*
 * CAN_DRIVER.h
 *
 * Created: 01.10.2015 13:49:47
 *  Author: haakongn
 */ 


#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

#include <stdint.h>

typedef struct CANMessage
{
	uint8_t ID;
	uint8_t length;
	uint8_t data_array[8];
} CANMessage;

void CAN_init();
uint8_t CAN_send_message(CANMessage message);
uint8_t CAN_receive_message(CANMessage* message);

#endif /* CAN_DRIVER_H_ */