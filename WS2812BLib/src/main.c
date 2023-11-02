/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "ws2812b.h"
#include "stm32f0xx.h"

uint32_t pos;
uint8_t color;

void TIM3_IRQHandler() {
	TIM3->SR &= ~TIM_SR_UIF;

	if (pos == WS2812B_N_PIXELS) {
		pos = 0;
		color = color + 1;

		if (color == 64) {
			color = 1;
		}
	}



	ws2812b_setpixel(pos, (color << (6 - 4)) & 0xc0,
			              (color << (6 - 2)) & 0xc0,
						  (color << (6 - 0)) & 0xc0);

	//ws2812b_setpixel(pos, 0, 0, 100);
	ws2812b_refresh();
	pos++;
}

int main(void) {
	ws2812b_init();


	pos = 0;
	color = 1;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->DIER  = TIM_DIER_UIE;
	TIM3->PSC   = 4800 - 1;
	TIM3->ARR   = 200 - 1;
	TIM3->CR1 |= TIM_CR1_CEN;
	NVIC->ISER[0] |= (1 << TIM3_IRQn);

	NVIC_SetPriority(TIM3_IRQn, 3);


	for(;;) {
		asm volatile ("wfi;");
	}
}
