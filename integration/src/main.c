

#include "HUB75.h"
#include "HUB75_font.h"
#include "SoundLib.h"
#include "stm32f0xx.h"

int number;

void TIM1_BRK_UP_TRG_COM_IRQHandler() {

	TIM1->SR &= ~TIM_SR_UIF;

	int color = (number % 7) + 1;

	hub75_font_render_int_7x5(number, 1, HUB75_WIDTH - 7, 16, color & 0x4, color & 0x2, color & 0x1, 0, 0, 0);

	if ((number & 0xf) == 0) {
		sound_play(440, SOUND_MAX_VOL, 0, 0, Sound_Single, SOUND_SECOND * 0.5);
	}

	number += 1;
}

int main(void) {
	hub75_init(30);
	sound_init();

	// HUB75 initial test.

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

	// TIM1 for other updates

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->DIER  = TIM_DIER_UIE;
	TIM1->PSC   = 48000 - 1;
	TIM1->ARR   = 200 - 1;
	TIM1->CR1 |= TIM_CR1_CEN;
	NVIC->ISER[0] |= (1 << TIM1_BRK_UP_TRG_COM_IRQn);

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 3);

	for(;;);
}
