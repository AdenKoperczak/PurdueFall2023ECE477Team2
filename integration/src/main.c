
#include "ws2812b.h"
#include "HUB75.h"
#include "HUB75_font.h"
#include "SoundLib.h"
#include "keypad.h"
#include "stm32f0xx.h"

int number;
int pos;
int wscolor;
int currentTone;

const int tones[][2] = {
		{277, 350},
		{311, 415},
		{330, 392},
		{261, 311},
		{350, 440}
};

int keyCount;
int lastKey;

void keypad_callback(int key) {
	switch (key) {
	case Keypad_None:
		hub75_font_render_7x5(' ', HUB75_WIDTH - 7, 5, 1, 1, 0, 0, 0, 0);
		break;
	case Keypad_Invl:
		hub75_font_render_7x5('I', HUB75_WIDTH - 7, 5, 1, 1, 0, 0, 0, 0);
		break;
	default:
		hub75_font_render_7x5(keypadChars[key], HUB75_WIDTH - 7, 5, 1, 1, 0, 0, 0, 0);
		break;
	}

	if (key != Keypad_None && key != Keypad_Invl && lastKey != Keypad_Invl && key != lastKey) {
		keyCount ++;
	}

	lastKey = key;

	hub75_font_render_int_7x5(keyCount, 0, 1, 16, 1, 1, 0, 0, 0, 0);
}

void TIM1_BRK_UP_TRG_COM_IRQHandler() {

	TIM1->SR &= ~TIM_SR_UIF;

	int color = (number % 7) + 1;

	hub75_font_render_int_7x5(number, 1, HUB75_WIDTH - 7, 16, color & 0x4, color & 0x2, color & 0x1, 0, 0, 0);


	if ((number & 0x3) == 0) {
		sound_play(tones[currentTone][0], SOUND_MAX_VOL, tones[currentTone][1], SOUND_MAX_VOL, Sound_Mixed, SOUND_SECOND * 0.5);
		currentTone++;
		if (currentTone == sizeof(tones) / sizeof(tones[0])) {
			currentTone = 0;
		}
	}


	TIM3->SR &= ~TIM_SR_UIF;

	if (pos == 14) {
		pos = 0;
		wscolor = wscolor + 1;

		if (wscolor == 64) {
			wscolor = 1;
		}
	}



	ws2812b_setpixel(pos, (wscolor << (7 - 3)) & 0xc0,
			              (wscolor << (7 - 1)) & 0xc0,
						  (wscolor << (7 - 0)) & 0xc0);

	ws2812b_refresh();
	pos++;

	number += 1;
}

int main(void) {
	pos = 0;
	wscolor = 1;
	number = 0;
	keyCount = 0;
	lastKey  = Keypad_None;

	ws2812b_init();
	hub75_init(30);
	sound_init();
	keypad_init();


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
