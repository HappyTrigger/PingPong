#ifndef _PING_PONG_LIBRARY_
#define _PING_PONG_LIBRARY_

#include <stdint.h>

#define OLED_COMMAND_BASE_ADDR 0x1000
#define OLED_DATA_BASE_ADDR 0x1200
#define ADC_BASE_ADDR 0x1400
#define SRAM_BASE_ADDR 0x1800

#define USERNAME_LENGTH 5
#define SRAM_USERNAME_ADDR SRAM_BASE_ADDR + OLED_ADDR_SPACE_LEN

#define HIGH_SCORE_SCORE_LENGTH 2
#define HIGH_SCORES_TABLE_LENGTH 5

#define SRAM_HIGH_SCORES_ADDR SRAM_USERNAME_ADDR + USERNAME_LENGTH
#define SRAM_HIGH_SCORE_LENGTH 13

#define EEPROM_HIGH_SCORES_BASE_ADDR 0x00

#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit)(reg & (1 << bit))

typedef struct high_score
{
	char username[USERNAME_LENGTH][HIGH_SCORES_TABLE_LENGTH];
	uint16_t score[HIGH_SCORES_TABLE_LENGTH];
} high_score;

void init();

void led_toggle();

void SRAM_test();

high_score high_score_read();

void high_score_write(high_score scores);

uint8_t high_score_add(char* name, uint16_t new_score);

void SRAM_high_score_write();

void high_score_clear();
#endif /*_PING_PONG_LIBRARY_*/