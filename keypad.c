#include "stm32f0xx.h"
#include <stdint.h>

void inita();
void initb();
void keypad(void);

void mysleep(void) {
    for(int n = 0; n < 1000; n++);
}

int main(void) {
    inita();
    initb();

     while(1) {
       keypad();
     }

    for(;;);
}

void inita() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_Mode_OUT << (5 << 1);
}

/**
 * @brief Init GPIO port C
 *        Pin 0-3: inputs with internal pull down resistors
 *        Pin 4-7: outputs
 *
 */
void initb() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // Only clear the mode of the pins we are going to use
    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);

    // Pull down resistor config
    // First reset the input pins resistors
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR0;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR1;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR2;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3;

    // Then set them to Pull-down
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR0_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR1_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR2_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR3_1;

    // PC4-7 are outputs
    GPIOB->MODER |= GPIO_Mode_OUT << (4 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (5 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (6 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (7 << 1);
}

//void initb() {
//    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
//
//    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 |
//                      GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
//
//    // Configure Pin 0-3 as outputs
//    GPIOB->MODER |= GPIO_Mode_OUT << (0 << 1);
//    GPIOB->MODER |= GPIO_Mode_OUT << (1 << 1);
//    GPIOB->MODER |= GPIO_Mode_OUT << (2 << 1);
//    GPIOB->MODER |= GPIO_Mode_OUT << (3 << 1);
//
//    // Pull down resistor config for Pin 4-7 (inputs)
//    // First reset the input pins resistors (PB4-7)
//    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR4;
//    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR5;
//    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR6;
//    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR7;
//
//    // Then set them to Pull-down
//    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR4_1;
//    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR5_1;
//    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_1;
//    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_1;
//}

void keypad(void) {
    for(int c = 8; c > 0; c = c >> 1) {
        GPIOB->ODR = c << 4;
        mysleep();
        int r = GPIOB->IDR & 0xf;
        //c=8 : col1, c=4 : col2, c=2 : col3, c=1 : col4
        //r=8 : row1, r=4 : row2, r=2 : row3, r=1 : row4
        if ((c == 8) | (c == 4) | (c == 2) | (c == 1)) {
            if(r & 1){
                GPIOA -> ODR |= 1<<5;
            }
            else if(r & 2){
                GPIOA -> ODR |= 1<<5;
            }
            else if(r & 4){
                GPIOA -> ODR |= 1<<5;
            }
            else if(r & 8){
                GPIOA -> ODR |= 1<<5;
            }
            else{
                GPIOA -> ODR &= ~(1<<5);
            }
        }
    }
}
