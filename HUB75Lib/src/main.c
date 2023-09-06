/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "HUB75.h"
#include "HUB75_font.h"

Hub75Pix pixels[HUB75_PIXELS];

int main(void) {
	hub75_init(pixels);

//#define FIRST
#define WHITE

#ifdef FIRST
	for (int x = 0; x < HUB75_WIDTH; x++) {
		hub75_set_pixel(pixels, x, 0, x & 1, x & 2, x & 4);
		hub75_set_pixel(pixels, x, 1, 1, 0, 0);
		hub75_set_pixel(pixels, x, 2, 0, 1, 0);
		hub75_set_pixel(pixels, x, 3, 0, 0, 1);

		hub75_set_pixel(pixels, x, HUB75_HIEGHT - 1 - 3, 0, 0, 1);
		hub75_set_pixel(pixels, x, HUB75_HIEGHT - 1 - 2, 0, 1, 0);
		hub75_set_pixel(pixels, x, HUB75_HIEGHT - 1 - 1, 1, 0, 0);
		hub75_set_pixel(pixels, x, HUB75_HIEGHT - 1 - 0, x & 1, x & 2, x & 4);

		hub75_font_render_8x6(pixels, '1', 5, 5, 1, 1, 1, 0, 0, 0);
		hub75_font_render_8x6(pixels, '0', 30, 5, 0, 1, 0, 0, 0, 0);
	}
#endif
#ifdef WHITE
	for (int y = 0; y < HUB75_HIEGHT; y++) {
		for (int x = 0; x < HUB75_WIDTH; x++) {
			hub75_set_pixel(pixels, x, y, 1, 1, 1);
		}
	}
#endif



	for(;;) {
		asm volatile ("wfi;");
	}
}
