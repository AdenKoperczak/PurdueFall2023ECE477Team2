#ifndef WS2812B_H
#define WS2812B_H

#define WS2812B_N_PIXELS     66
#define WS2812B_N_BIT_VALUES 64

#define WS2812B_CLOCK_PRE (19 - 1)
#define WS2812B_CLOCK_ARR 2
#include "stm32f0xx.h"


// This math ensures there are always a multiple of 4 bytes/32 bits
// This is needed for how this is done. writing extra does not hurt
#define WS2812B_N_BYTES ((4 * WS2812B_N_PIXELS) - ((WS2812B_N_PIXELS / 4) * 4))

#define WS2812B_N_WORDS (WS2812B_N_BYTES / 4)
#define WS2812B_N_BITS  8 * WS2812B_N_BYTES
#define WS2812B_IDLE    0


void ws2812b_init();
void ws2812b_setpixel(uint32_t i, uint8_t r, uint8_t g, uint8_t b);
void ws2812b_refresh();

#endif
