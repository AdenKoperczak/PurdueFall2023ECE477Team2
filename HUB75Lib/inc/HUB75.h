#ifndef __HUB75_H
#define __HUB75_H
#include "stm32f0xx.h"

#define HUB75_WIDTH  64
#define HUB75_HIEGHT 32
#define HUB75_ROW_END 3

#define HUB75_CR_EN  (1 << 5)
#define HUB75_CR_LAT (1 << 4)

#define HUB75_CLOCK_DIV TIM_CR1_CMS_0
#define HUB75_CLOCK_PRE 0
#define HUB75_CLOCK_ARR 0xff
#define HUB75_CLOCK_CCR (HUB75_CLOCK_ARR / 2)

#define HUB75_ROWS (HUB75_HIEGHT / 2)
#define HUB75_COLUMNS (HUB75_WIDTH + HUB75_ROW_END)
#define HUB75_PIXELS (HUB75_ROWS * HUB75_COLUMNS)

typedef struct {
	uint8_t color;
	uint8_t cr;
} Hub75Pix;

void hub75_init(Hub75Pix * pixels);
void hub75_set_pixel(Hub75Pix * pixels, int x, int y, uint8_t r, int8_t g, int8_t b);

#endif
