#include "stm32f0xx.h"
#include "HUB75.h"

void hub75_init(Hub75Pix * pixels) {
	// init the pixel buffer
	for (int row = 0; row < HUB75_ROWS; row++) {
		int column;
		int rowOffset = row * HUB75_COLUMNS;

		for (column = 0; column < HUB75_WIDTH; column++) {
			pixels[rowOffset + column].color = 0;
			pixels[rowOffset + column].cr    = 0 | HUB75_CR_EN;
		}

		pixels[rowOffset + column + 0].color = 0;
		pixels[rowOffset + column + 0].cr    = 0xf & (row - 1);
		pixels[rowOffset + column + 1].color = 0;
		pixels[rowOffset + column + 1].cr    = (0xf & (row - 1)) | HUB75_CR_LAT;
		pixels[rowOffset + column + 2].color = 0;
		pixels[rowOffset + column + 2].cr    = 0xf & (row - 1);
	}

	// init the GPIO pins
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER = 0x55552555;

	// init timer 3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1   = HUB75_CLOCK_DIV | TIM_CR1_UDIS;
	TIM3->DIER  = TIM_DIER_TDE | TIM_DIER_CC1DE;
	TIM3->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;
	TIM3->CCER  = TIM_CCER_CC1E;
	TIM3->PSC   = HUB75_CLOCK_PRE;
	TIM3->ARR   = HUB75_CLOCK_ARR;
	TIM3->CCR1  = HUB75_CLOCK_CCR;

	// init the DMA
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	DMA1_Channel4->CCR = DMA_CCR_PL | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 |
			             DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_DIR;
	DMA1_Channel4->CNDTR = HUB75_PIXELS;
	DMA1_Channel4->CPAR = (int32_t)&(GPIOC->ODR);
	DMA1_Channel4->CMAR = (int32_t)pixels;

	// Start the output process
	DMA1_Channel4->CCR |= DMA_CCR_EN;
	TIM3->CR1 |= TIM_CR1_CEN;
}

void hub75_set_pixel(Hub75Pix * pixels, int x, int y, uint8_t r, int8_t g, int8_t b) {
	r = r ? 1 : 0;
	g = g ? 1 : 0;
	b = b ? 1 : 0;

	if (y < HUB75_ROWS) {
		pixels[y * HUB75_COLUMNS + x].color = (pixels[y * HUB75_COLUMNS + x].color & 0x38) | r<<2 | g<<1 | b<<0;
	} else {
		pixels[(y - HUB75_ROWS) * HUB75_COLUMNS + x].color = (pixels[(y - HUB75_ROWS) * HUB75_COLUMNS + x].color & 0x07) | r<<5 | g<<4 | b<<3;
	}
}







