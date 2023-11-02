#include "ws2812b.h"
#include "stm32f0xx.h"


uint8_t ws2812bBitValues [WS2812B_N_BIT_VALUES];
uint8_t ws2812bPixels    [WS2812B_N_BYTES];
uint32_t * ws2812bPixWords = (uint32_t *)ws2812bPixels;

uint32_t ws2812bOffset;
uint8_t ws2812bUpdate;

void DMA1_CH2_3_DMA2_CH1_2_IRQHandler() {
	if (!(DMA1->ISR & (DMA_ISR_HTIF2 | DMA_ISR_TCIF2))) {
		return;
	}

	uint8_t * nextBit = &ws2812bBitValues[(DMA1->ISR & DMA_ISR_HTIF2) ? 0 : 32]; // Bitwise could make this faster.
	DMA1->IFCR = DMA_IFCR_CHTIF2 | DMA_IFCR_CTCIF2;

	if (ws2812bOffset == 0) {
		ws2812bUpdate = 0;
	}

	if (ws2812bOffset < WS2812B_N_WORDS) {
		uint32_t bits = ws2812bPixWords[ws2812bOffset];

		for (uint32_t b = 0; b < 4; b++) {
			for (uint32_t i = 0; i < 8; i++) {
				(*nextBit) = 1 + ((bits >> ((b << 3) + 8 - i)) & 0x1);
				nextBit++;
			}
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

	NVIC_EnableIRQ(10);
	NVIC_SetPriority(10, 2);

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















