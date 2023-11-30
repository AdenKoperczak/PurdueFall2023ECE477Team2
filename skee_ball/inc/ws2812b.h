#ifndef WS2812B_H
#define WS2812B_H

#define WS2812B_N_PIXELS     66
#define WS2812B_N_BIT_VALUES 64

#define WS2812B_CLOCK_PRE (19 - 1)
#define WS2812B_CLOCK_ARR 2
#include "stm32f0xx.h"
#include "GameLogic.h"
#include "colors.h"

// This math ensures there are always a multiple of 4 bytes/32 bits
// This is needed for how this is done. writing extra does not hurt
#define WS2812B_N_BYTES ((4 * WS2812B_N_PIXELS) - ((WS2812B_N_PIXELS / 4) * 4))

#define WS2812B_N_WORDS (WS2812B_N_BYTES / 4)
#define WS2812B_N_BITS  8 * WS2812B_N_BYTES
#define WS2812B_IDLE    0


typedef enum {
	L_Mode_Off,
	L_Mode_Solid,
	L_Mode_Slow_Flash,
	L_Mode_Fast_Flash,
	L_Mode_Chase,
	L_Mode_Select
} LightsMode;

void ws2812b_init();
void ws2812b_setpixel(uint32_t i, uint8_t r, uint8_t g, uint8_t b);
void ws2812b_refresh();

void lights_set_mode(LightsMode mode, uint8_t r, uint8_t g, uint8_t b);
void lights_tick();

#endif
