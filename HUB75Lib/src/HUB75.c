#include "stm32f0xx.h"
#include "HUB75.h"

Hub75Pix pixels[HUB75_PIXELS];

void hub75_init(int brightness) {
	// init the pixel buffer
	for (int row = 0; row < HUB75_ROWS; row++) {
		int column;
		int rowOffset = row * HUB75_COLUMNS;
		int rowValue = (0xf & (row - 1));

		for (column = 0; column < HUB75_WIDTH; column++) {
			pixels[rowOffset + column].color = HUB75_COLOR_CEN;
			pixels[rowOffset + column].cr    = rowValue | ((brightness > column) ? 0 : HUB75_CR_ODE);
		}

		pixels[rowOffset + column + 0].color = 0;
		pixels[rowOffset + column + 0].cr    = rowValue | HUB75_CR_ODE;
		pixels[rowOffset + column + 1].color = 0;
		pixels[rowOffset + column + 1].cr    = rowValue | HUB75_CR_LAT | HUB75_CR_ODE;
		pixels[rowOffset + column + 2].color = 0;
		pixels[rowOffset + column + 2].cr    = rowValue | HUB75_CR_ODE;
	}

	// init the GPIO pins
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER = 0x55556555;

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

void hub75_set_pixel(int x, int y, uint8_t r, int8_t g, int8_t b) {
	if (y < HUB75_ROWS) {
		r = r ? HUB75_COLOR_R0 : 0;
		g = g ? HUB75_COLOR_G0 : 0;
		b = b ? HUB75_COLOR_B0 : 0;

		pixels[y * HUB75_COLUMNS + x].color = (pixels[y * HUB75_COLUMNS + x].color & 0xb8) | r | g | b;
	} else {
		y = y - HUB75_ROWS;

		r = r ? HUB75_COLOR_R1 : 0;
		g = g ? HUB75_COLOR_G1 : 0;
		b = b ? HUB75_COLOR_B1 : 0;

		pixels[y * HUB75_COLUMNS + x].color = (pixels[y * HUB75_COLUMNS + x].color & 0x87) | r | g | b;
	}
}







