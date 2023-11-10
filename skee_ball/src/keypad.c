#include "stm32f0xx.h"
#include <stdint.h>
#include "keypad.h"


volatile int keypadKey;

int currentPressed;
int column;

const char keypadChars[16] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'*', '0', '#', 'D'
};

void keypad_init() {
	column = 3;
	currentPressed = Keypad_None;

    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);

    // Configure Pin 0-3 as outputs'

    GPIOB->MODER |= GPIO_Mode_OUT << (0 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (1 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (2 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (3 << 1);


    // initialize for first loop
	GPIOB->ODR = (1<<column) & 0xf;

	// init timer 16
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	TIM16->CR1   = KEYPAD_CLOCK_DIV | TIM_CR1_UDIS;
	TIM16->DIER  = TIM_DIER_UIE;
	TIM16->PSC   = KEYPAD_CLOCK_PRE;
	TIM16->ARR   = KEYPAD_CLOCK_ARR;
	TIM16->CR1  &= ~TIM_CR1_UDIS;
	TIM16->CR1  |= TIM_CR1_CEN;

	NVIC_EnableIRQ(TIM16_IRQn);
	NVIC_SetPriority(TIM16_IRQn, 3);
}

void TIM16_IRQHandler (void) {
	TIM16->SR &= ~TIM_SR_UIF;

	int r = (GPIOB->IDR >> 4) & 0xf;
	//column=8 : col1, column=4 : col2, column=2 : col3, column=1 : col4
	//r=8 : row1, r=4 : row2, r=2 : row3, r=1 : row4
	if (r != 0 && currentPressed != Keypad_None) {
		currentPressed = Keypad_Invl;
	} else {
		switch (r) {
		case 0:
			break;
		case 1:
			currentPressed = 15 - 0 - column * 4;
			break;
		case 2:
			currentPressed = 15 - 1 - column * 4;
			break;
		case 4:
			currentPressed = 15 - 2 - column * 4;
			break;
		case 8:
			currentPressed = 15 - 3 - column * 4;
			break;
		default:
			currentPressed = Keypad_Invl;
		}
	}


	if (column == 0) {
		column = 3;
		keypadKey = currentPressed;
		currentPressed = Keypad_None;
		keypad_callback(keypadKey);
	} else {
		column--;
	}


	GPIOB->ODR = (1<<column) & 0xf;
}
