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



int number;

void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;

	int color = (number % 7) + 1;

	hub75_font_render_int_7x5(number, 1, HUB75_WIDTH - 7, 16, color & 0x4, color & 0x2, color & 0x1, 0, 0, 0);

	number += 1;
}

int main(void) {
	hub75_init(32);

#define FIRST
//#define WHITE

	number = 0;
#ifdef FIRST
	for (int x = 0; x < HUB75_WIDTH; x++) {
		hub75_set_pixel(x, 0, x & 1, x & 2, x & 4);
		hub75_set_pixel(x, 1, 1, 0, 0);
		hub75_set_pixel(x, 2, 0, 1, 0);
		hub75_set_pixel(x, 3, 0, 0, 1);

		hub75_set_pixel(x, HUB75_HIEGHT - 1 - 3, 0, 0, 1);
		hub75_set_pixel(x, HUB75_HIEGHT - 1 - 2, 0, 1, 0);
		hub75_set_pixel(x, HUB75_HIEGHT - 1 - 1, 1, 0, 0);
		hub75_set_pixel(x, HUB75_HIEGHT - 1 - 0, x & 1, x & 2, x & 4);

	}

	hub75_font_render_str_7x5("DEADBEEF", 1, 5, 1, 1, 1, 0, 0, 0);

	// init for animation

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->DIER  = TIM_DIER_UIE;
	TIM2->PSC   = 48000 - 1;
	TIM2->ARR   = 200 - 1;
	TIM2->CR1 |= TIM_CR1_CEN;
	NVIC->ISER[0] |= (1 << TIM2_IRQn);

	NVIC_SetPriority(TIM2_IRQn, 3);

#endif
#ifdef WHITE
	for (int y = 0; y < HUB75_HIEGHT; y++) {
		for (int x = 0; x < HUB75_WIDTH; x++) {
			hub75_set_pixel(x, y, 1, 1, 1);
		}
	}
#endif



	for(;;) {
		asm volatile ("wfi;");
	}
}
