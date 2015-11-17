/*
 * MCP_DRIVER.h
 *
 * Created: 01.10.2015 12:51:17
 *  Author: haakongn
 */ 


#ifndef MCP_DRIVER_H_
#define MCP_DRIVER_H_

#include "MCP_ADDRESSES.h"
#include <stdint.h>

void mcp_reset();
void mcp_turn_on();
void mcp_turn_off();

uint8_t mcp_init();

uint8_t mcp_read(uint8_t address);
uint8_t mcp_read_status();

void mcp_write_byte(uint8_t address, uint8_t byte);
void mcp_modify_bit(uint8_t address, uint8_t mask, uint8_t byte);
void mcp_request_to_send(uint8_t bits);


#endif /* MCP_DRIVER_H_ */