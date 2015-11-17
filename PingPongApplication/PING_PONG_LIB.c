/*
 * CFile1.c
 *
 * Created: 04.09.2015 15:35:58
 *  Author: michalma
 */ 

#include "PING_PONG_LIB.h"
#include "OLED_DRIVER.h"
#include "USART_DRIVER.h"
#include "USER_INTERFACE.h"
#include "EEPROM.h"
#include "TIME_DRIVER.h"
#include "CAN_DRIVER.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>


/****************************************************************************
* \brief Initialize all important peripherals and pins
*
****************************************************************************/
void init()
{
	USART_init(MYUBBR);
	/* Enables standard IO functions */
	fdevopen(USART_putchar, USART_getchar);
	/* Enables external memory*/
	set_bit(MCUCR, SRE);
	/* Pin 1 is set to output for the LED*/
	set_bit(DDRB, PB0);
	/* Pin is now driven LOW*/
	clear_bit(PORTB, PB0);
	/* Initialize OLED display */
	init_oled();
	timer1_init();
	CAN_init();
	interface_init();
	clear_oled();
	reset_position();
	refresh_oled();
	sei();
}

/****************************************************************************
* \brief Toggles the led connected to pin 1 (PORTB, PB0)
*
****************************************************************************/
void led_toggle()
{
	if (test_bit(PINB, PB0))
	{
		clear_bit(PORTB, PB0);
	}
	else
	{
		set_bit(PORTB, PB0);
	}
}


/****************************************************************************
* \brief Test function for the SRAM and Gal chip
*
****************************************************************************/
void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800;	//	Start address for the SRAM
	
	unsigned int i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	unsigned char testvalue;
	printf("Starting SRAM test...\r\n");
	
	
	for (i = 0; i < 0x400; i++)
	{
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue)
		{
			printf("SRAM error (write phase): ext_ram[%d] = %02X (should be	%02X)\r\n", i, ext_ram[i], testvalue);
			werrors++;
		}
	}
	
	for (i = 0; i < 0x400; i++)
	{
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue)
		{
			printf("SRAM error (read phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			rerrors++;
		}
	}
	printf("SRAM test completed with %d errors in write phase and %d errors in read phase\r\n", werrors, rerrors);
}

/****************************************************************************
* \brief Read high scores from EEPROM
*
****************************************************************************/
high_score high_score_read()
{
	high_score scores;
	uint8_t i, j;
	// Read usernames
	for (i = 0; i < HIGH_SCORES_TABLE_LENGTH; i++)
	{
		for (j = 0; j < USERNAME_LENGTH; j++)
		{
			scores.username[i][j] = EEPROM_read(EEPROM_HIGH_SCORES_BASE_ADDR + i * USERNAME_LENGTH + j);
		}
	}
	// Read high scores
	for (i = 0; i < HIGH_SCORES_TABLE_LENGTH; i++)
	{
		scores.score[i] = (uint16_t) EEPROM_read(EEPROM_HIGH_SCORES_BASE_ADDR + HIGH_SCORES_TABLE_LENGTH * USERNAME_LENGTH + (2 * i));
		scores.score[i] <<= 8;
		scores.score[i] |= (uint16_t) EEPROM_read(EEPROM_HIGH_SCORES_BASE_ADDR + HIGH_SCORES_TABLE_LENGTH * USERNAME_LENGTH + (2 * i) + 1);
	}
	
	return scores;
}

/****************************************************************************
* \brief Write high scores to EEPROM
* \param in High scores
*
****************************************************************************/
void high_score_write(high_score scores)
{
	uint8_t i, j;
	// Write usernames
	for (i = 0; i < HIGH_SCORES_TABLE_LENGTH; i++)
	{
		for (j = 0; j < USERNAME_LENGTH; j++)
		{
			EEPROM_write(EEPROM_HIGH_SCORES_BASE_ADDR + i * USERNAME_LENGTH + j, scores.username[i][j]);
		}
	}
	// Write high scores
	for (i = 0; i < HIGH_SCORES_TABLE_LENGTH; i++)
	{
		EEPROM_write(EEPROM_HIGH_SCORES_BASE_ADDR + HIGH_SCORES_TABLE_LENGTH * USERNAME_LENGTH + (2 * i), (uint8_t) ((scores.score[i] >> 8) & 0xFF));
		EEPROM_write(EEPROM_HIGH_SCORES_BASE_ADDR + HIGH_SCORES_TABLE_LENGTH * USERNAME_LENGTH + (2 * i) + 1, (uint8_t) (scores.score[i] & 0xFF));
	}
}

/****************************************************************************
* \brief Rewrite high scores from EEPROM to SRAM
*
****************************************************************************/
void SRAM_high_score_write()
{
	volatile uint8_t* high_scores_sram = (uint8_t*) SRAM_HIGH_SCORES_ADDR;
	char buffer[6];
	uint8_t len, i, j;
	high_score scores = high_score_read();
	
	for(i = 0; i < HIGH_SCORES_TABLE_LENGTH; i ++)
	{
		snprintf(buffer, 6, "%u", scores.score[i]);
		len = strlen(buffer);
		
		for (j = 0; j < USERNAME_LENGTH; j++)
		{
			high_scores_sram[SRAM_HIGH_SCORE_LENGTH * i + j] = scores.username[i][j];
		}
		
		high_scores_sram[SRAM_HIGH_SCORE_LENGTH * i + USERNAME_LENGTH] = ' ';
		high_scores_sram[SRAM_HIGH_SCORE_LENGTH * i + USERNAME_LENGTH + 1] = ' ';
		high_scores_sram[SRAM_HIGH_SCORE_LENGTH * i + USERNAME_LENGTH + 2] = ' ';
		
		for (j = 0; j < (5 - len); j ++)
		{
			high_scores_sram[SRAM_HIGH_SCORE_LENGTH * i + USERNAME_LENGTH + 3 + j] = ' ';
		}
		
		
		for (j = 0; j < len; j++)
		{
			high_scores_sram[SRAM_HIGH_SCORE_LENGTH * i + USERNAME_LENGTH + 3 + (5 - len) + j] = buffer[j];
		}
	}
}

/****************************************************************************
* \brief Try to add new high score
* \param in Username
* \param in Score
* \param out 0 if addition wasn't successful else position of new high score
****************************************************************************/
uint8_t high_score_add(char* name, uint16_t new_score)
{
	uint8_t i, j, k;
	high_score scores = high_score_read();
	
	for (i = 0; i < HIGH_SCORES_TABLE_LENGTH; i++)
	{
		if (new_score > scores.score[i])
		{
			for (j = HIGH_SCORES_TABLE_LENGTH - 1; j > i; j--)
			{
				scores.score[j] = scores.score[j - 1];
				for (k = 0; k < USERNAME_LENGTH; k++)
				{
					scores.username[j][k] = scores.username[j - 1][k];
				}
			}
			scores.score[i] = new_score;
			for (k = 0; k < USERNAME_LENGTH; k++)
			{
				scores.username[i][k] = name[k];
			}
			high_score_write(scores);
			SRAM_high_score_write();
			return i + 1;
		}
	}
	
	return 0;
}


void high_score_clear()
{
	
	high_score scores;
	strncpy ( scores.username[0], "     \0", 6 );
	strncpy ( scores.username[1], "     \0", 6 );
	strncpy ( scores.username[2], "     \0", 6 );
	strncpy ( scores.username[3], "     \0", 6 );
	strncpy ( scores.username[4], "     \0", 6 );
	scores.score[0] = 0x0000;
	scores.score[1] = 0x0000;
	scores.score[2] = 0x0000;
	scores.score[3] = 0x0000;
	scores.score[4] = 0x0000;
	high_score_write(scores);
}