#include "ws2812b.h"
#include "stm32f0xx.h"


uint8_t ws2812bBitValues [WS2812B_N_BIT_VALUES];
uint8_t ws2812bPixels    [WS2812B_N_BYTES];

uint32_t ws2812bOffset;
uint8_t ws2812bUpdate;

void DMA1_CH2_3_DMA2_CH1_2_IRQHandler() {
	// This function is quite well optimized and not necessarily readable
	if (!(DMA1->ISR & (DMA_ISR_HTIF2 | DMA_ISR_TCIF2))) {
		return;
	}

	uint8_t * nextBit = ws2812bBitValues + (DMA1->ISR & DMA_ISR_TCIF2); // Whack. Only works because DMA_ISR_TCIF2 == 32
	DMA1->IFCR = DMA_IFCR_CHTIF2 | DMA_IFCR_CTCIF2;

	if (ws2812bOffset == 0) {
		ws2812bUpdate = 0;
	}

	if (ws2812bOffset < WS2812B_N_WORDS) {
		for (uint32_t b = (ws2812bOffset << 2) + 0; b < (ws2812bOffset << 2) + 4; b++) {
			// manually unraveled loop. Nice. (was on -O0, may not be needed on -O2, but fight me - Aden)
			uint8_t pixelByte = ws2812bPixels[b];
			(*nextBit) = 1 + ((pixelByte >> 7) & 0x1);
			nextBit++;
			(*nextBit) = 1 + ((pixelByte >> 6) & 0x1);
			nextBit++;
			(*nextBit) = 1 + ((pixelByte >> 5) & 0x1);
			nextBit++;
			(*nextBit) = 1 + ((pixelByte >> 4) & 0x1);
			nextBit++;
			(*nextBit) = 1 + ((pixelByte >> 3) & 0x1);
			nextBit++;
			(*nextBit) = 1 + ((pixelByte >> 2) & 0x1);
			nextBit++;
			(*nextBit) = 1 + ((pixelByte >> 1) & 0x1);
			nextBit++;
			(*nextBit) = 1 + ((pixelByte >> 0) & 0x1);
			nextBit++;
		}

		ws2812bOffset++;
	} else if (*nextBit != WS2812B_IDLE) {
		for (uint32_t i = 0; i < 32; i++) {
			(*nextBit) = WS2812B_IDLE;

			nextBit++;
		}
	} else {
		ws2812bOffset = 0;
		// this is done as such to avoid hazards
		TIM2->CR1 &= ~TIM_CR1_CEN; // disable the timer

		if (ws2812bUpdate) {
			TIM2->CR1 |= TIM_CR1_CEN;// reenable the timer if more updates are needed
		}
	}
}

void ws2812b_init() {
	// setup memory
	for (int i = 0; i < WS2812B_N_BIT_VALUES; i++) {
		ws2812bBitValues[i] = WS2812B_IDLE;
	}
	for (int i = 0; i < WS2812B_N_BYTES; i++) {
		ws2812bPixels[i] = 0;
	}
	ws2812bOffset = 0;

	// init GPIO Port A
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->OSPEEDR |= 0x3 << (3 * 2);
	GPIOA->MODER |= 0x2 << (3 * 2);
	GPIOA->AFR[0] |= 0x2 << (3 * 4);

	// init timer 2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1   = TIM_CR1_UDIS | TIM_CR1_URS; // TIM_CR1_ARPE;
	TIM2->CR2   = TIM_CR2_CCDS;
	TIM2->DIER  = TIM_DIER_TDE | TIM_DIER_UDE;
	TIM2->CCMR2 = TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE; //
	TIM2->CCER  = TIM_CCER_CC4E;
	TIM2->PSC   = WS2812B_CLOCK_PRE;
	TIM2->ARR   = WS2812B_CLOCK_ARR;
	TIM2->CCR4  = WS2812B_IDLE;

	TIM2->CR1 = (TIM2->CR1) & ~TIM_CR1_UDIS;


	// Init the DMA
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	DMA1_Channel2->CCR = DMA_CCR_PL | DMA_CCR_MINC |
			             DMA_CCR_PSIZE_1 | DMA_CCR_CIRC | DMA_CCR_DIR |
			             DMA_CCR_HTIE | DMA_CCR_TCIE;
	DMA1_Channel2->CNDTR = WS2812B_N_BIT_VALUES;
	DMA1_Channel2->CPAR = (int32_t)&(TIM2->CCR4);
	DMA1_Channel2->CMAR = (int32_t)ws2812bBitValues;

	NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
	NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0);

	// Enable DMA and Timer
	DMA1_Channel2->CCR |= DMA_CCR_EN;
	ws2812bUpdate       = 1;
	TIM2->CR1          |= TIM_CR1_CEN;
}


void ws2812b_setpixel(uint32_t i, uint8_t r, uint8_t g, uint8_t b) {
	if (i < WS2812B_N_PIXELS) {
		ws2812bPixels[i * 3 + 0] = g;
		ws2812bPixels[i * 3 + 1] = r;
		ws2812bPixels[i * 3 + 2] = b;
	}
}

void ws2812b_refresh() {
	ws2812bUpdate = 1;
	TIM2->CR1 |= TIM_CR1_CEN;
}

#define LIGHTS_SLOW_FLASH_PERIOD (int)(GAME_SECOND * 1.0)
#define LIGHTS_FAST_FLASH_PERIOD (int)(GAME_SECOND * 0.5)

LightsMode lightsMode;
uint8_t lightsR, lightsG, lightsB;
uint8_t lightsOff;
int lightsTime;

void lights_set_mode(LightsMode mode, uint8_t r, uint8_t g, uint8_t b) {
	if (mode != lightsMode || r != lightsR || g != lightsG || b != lightsB) {
		lightsTime = 0;
	}

	lightsMode = mode;
	lightsR = r;
	lightsG = g;
	lightsB = b;

	if (mode != L_Mode_Off) {
		lightsOff = 0;
	}
}

void lights_off() {
	if (!lightsOff) {
		for (int i = 0; i < WS2812B_N_PIXELS; i++) {
			ws2812b_setpixel(i, 0, 0, 0);
		}
		ws2812b_refresh();
		lightsOff = 1;
	}
}

void lights_solid() {
	if (lightsTime == 0) {
		for (int i = 0; i < WS2812B_N_PIXELS; i++) {
			ws2812b_setpixel(i, lightsR, lightsG, lightsB);
		}
		ws2812b_refresh();
		lightsTime = 1;
	}
}

void lights_slow_flash() {

	if (lightsTime == 0) {
		for (int i = 0; i < WS2812B_N_PIXELS; i++) {
			ws2812b_setpixel(i, lightsR, lightsG, lightsB);
		}
		ws2812b_refresh();
	} else if (lightsTime == LIGHTS_SLOW_FLASH_PERIOD / 2) {

		for (int i = 0; i < WS2812B_N_PIXELS; i++) {
			ws2812b_setpixel(i, 0, 0, 0);
		}
		ws2812b_refresh();
	}

	lightsTime++;
	if (lightsTime == LIGHTS_SLOW_FLASH_PERIOD) {
		lightsTime = 0;
	}
}

void lights_fast_flash() {

	if (lightsTime == 0) {
		for (int i = 0; i < WS2812B_N_PIXELS; i++) {
			ws2812b_setpixel(i, lightsR, lightsG, lightsB);
		}
		ws2812b_refresh();
	} else if (lightsTime == LIGHTS_FAST_FLASH_PERIOD / 2) {

		for (int i = 0; i < WS2812B_N_PIXELS; i++) {
			ws2812b_setpixel(i, 0, 0, 0);
		}
		ws2812b_refresh();
	}

	lightsTime++;
	if (lightsTime == LIGHTS_FAST_FLASH_PERIOD) {
		lightsTime = 0;
	}
}

void lights_chase() {
	int i;
	for (i = 0; i < WS2812B_N_PIXELS / 2; i++) {
		if (((i + lightsTime) & 0xf) < 8) {
			ws2812b_setpixel(i, 0, 0, 0);
		} else {
			ws2812b_setpixel(i, lightsR, lightsG, lightsB);
		}
	}
	for (; i < WS2812B_N_PIXELS; i++) {
			if (((i - lightsTime + 8) & 0xf) < 8) {
				ws2812b_setpixel(i, 0, 0, 0);
			} else {
				ws2812b_setpixel(i, lightsR, lightsG, lightsB);
			}
		}
	ws2812b_refresh();
	lightsTime++;
}

void lights_select_sub(int i, int a) {
	switch (a) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
		ws2812b_setpixel(i, MODE_NORMAL_COLOR);
		break;
	case 0x08:
	case 0x09:
	case 0x0a:
	case 0x0b:
	case 0x0c:
	case 0x0d:
	case 0x0e:
	case 0x0f:
		ws2812b_setpixel(i, MODE_TIME_ATTACK_COLOR);
		break;
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
		ws2812b_setpixel(i, MODE_STREAK_COLOR);
		break;
	case 0x18:
	case 0x19:
	case 0x1a:
	case 0x1b:
	case 0x1c:
	case 0x1d:
	case 0x1e:
	case 0x1f:
		ws2812b_setpixel(i, MODE_COMBO_COLOR);
		break;
	default:
		ws2812b_setpixel(i, 0x00, 0x00, 0x00);
	}
}

void lights_select() {
	int i;
	for (i = 0; i < WS2812B_N_PIXELS / 2; i++) {
		lights_select_sub(i, (i + lightsTime) & 0x1f);
	}
	for (; i < WS2812B_N_PIXELS; i++) {
		lights_select_sub(i, (-i + lightsTime) & 0x1f);
	}
	ws2812b_refresh();
	lightsTime++;
}

void lights_tick() {
	switch (lightsMode) {
	case L_Mode_Slow_Flash:
		lights_slow_flash();
		break;
	case L_Mode_Fast_Flash:
		lights_fast_flash();
		break;
	case L_Mode_Solid:
		lights_solid();
		break;
	case L_Mode_Chase:
		lights_chase();
		break;
	case L_Mode_Select:
		lights_select();
		break;
	case L_Mode_Off:
	default:
		lights_off();
		break;
	}
}














