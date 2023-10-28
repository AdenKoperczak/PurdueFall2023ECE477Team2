#include "stm32f0xx.h"
#include <stdint.h>

void setup_tim15(void);
void initc();

void togglexn(GPIO_TypeDef *port, int n);

extern char keymap;
char* keymap_arr = &keymap;
extern uint8_t col;

int main(void) {
    initb();
    setup_tim15();
    for(;;);
}

void initc() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    //clear bit
    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);

    // all input mode
    GPIOB->MODER |= GPIO_Mode_IN << 0;
    GPIOB->MODER |= GPIO_Mode_IN << (1 << 1);
    GPIOB->MODER |= GPIO_Mode_IN << (2 << 1);
    GPIOB->MODER |= GPIO_Mode_IN << (3 << 1);
    GPIOB->MODER |= GPIO_Mode_IN << (4 << 1);
    GPIOB->MODER |= GPIO_Mode_IN << (5 << 1);
    GPIOB->MODER |= GPIO_Mode_IN << (6 << 1);
    GPIOB->MODER |= GPIO_Mode_IN << (7 << 1);

    //clear bit
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR0;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR1;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR2;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR4;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR5;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR6;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR7;

    // Pull-down mode
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR0_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR1_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR2_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR3_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR4_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR5_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_1;


}
void togglexn(GPIO_TypeDef *port, int n) {
    port->ODR = port->ODR ^ (1 << n);
}

char rows_to_key(int rows) {
    int n;

    if(rows == 0b0001){
        n = (col % 4) * 4;
    }
    else if(rows == 0b0010){
        n = ((col % 4) * 4 + 1);
    }
    else if(rows == 0b0100){
        n = ((col % 4) * 4 + 2);
    }
    else{
        n = ((col % 4) * 4 + 3);
    }
    return keymap_arr[n];
}
void setup_tim15(void) {

    GPIOC -> MODER &= 0xfff00fff;
    GPIOC -> MODER |= 0xaa000;

    RCC -> APB2ENR |= RCC_APB2ENR_TIM15EN;
    TIM15 -> PSC = 48000-1;
    TIM15 -> ARR = 500-1;
    TIM15 -> DIER |= TIM_DIER_UIE;
    TIM15 -> CR1 |= 1; //TIM15_CR1_CEN
}
