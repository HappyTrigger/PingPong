#ifndef _PING_PONG_LIBRARY_
#define _PING_PONG_LIBRARY_

#define BLINK_TIME 1000

#define OLED_COMMAND_BASE_ADDR 0x1000
#define OLED_DATA_BASE_ADDR 0x1200
#define ADC_BASE_ADDR 0x1400
#define SRAM_BASE_ADDR 0x1800
#define SRAM_USERNAME_ADDR SRAM_BASE_ADDR + OLED_ADDR_SPACE_LEN 
#define SRAM_USERNAME_ADDR_LENGTH 5

//#define ABS(val) ((val) > 0 ? val : (-val))

#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit)(reg & (1 << bit))

void init();

void led_toggle();

void SRAM_test();

#endif /*_PING_PONG_LIBRARY_*/