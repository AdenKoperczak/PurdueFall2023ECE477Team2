/**
  ******************************************************************************
  * @file    sensor.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   .
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "sensors.h"

void sensors_enable() {
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void sensors_disable() {
	NVIC_DisableIRQ(EXTI4_15_IRQn);
}

void sensors_init() {
    // init GPIOB
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODER8  | GPIO_MODER_MODER9  |
                      GPIO_MODER_MODER10 | GPIO_MODER_MODER11 |
                      GPIO_MODER_MODER12 | GPIO_MODER_MODER13 |
                      GPIO_MODER_MODER14);
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR8_1  | GPIO_PUPDR_PUPDR9_1  |
                    GPIO_PUPDR_PUPDR10_1 | GPIO_PUPDR_PUPDR11_1 |
                    GPIO_PUPDR_PUPDR12_1 | GPIO_PUPDR_PUPDR13_1 |
                    GPIO_PUPDR_PUPDR14_1;

    // init exti
    RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
    //change to number or port to
    SYSCFG -> EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PB  | SYSCFG_EXTICR3_EXTI9_PB |
                            SYSCFG_EXTICR3_EXTI10_PB | SYSCFG_EXTICR3_EXTI11_PB);
    SYSCFG -> EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PB | SYSCFG_EXTICR4_EXTI13_PB |
                            SYSCFG_EXTICR4_EXTI14_PB);
    EXTI->RTSR &= ~(EXTI_RTSR_TR8  | EXTI_RTSR_TR9  | EXTI_RTSR_TR10 |
                    EXTI_RTSR_TR11 | EXTI_RTSR_TR12 | EXTI_RTSR_TR13);
    EXTI->FTSR |= (EXTI_FTSR_TR8  | EXTI_FTSR_TR9  | EXTI_FTSR_TR10 |
                   EXTI_FTSR_TR11 | EXTI_FTSR_TR12 | EXTI_FTSR_TR13);
    EXTI -> RTSR |= EXTI_RTSR_TR14;

    EXTI -> IMR |= (EXTI_IMR_MR8  | EXTI_IMR_MR9  | EXTI_IMR_MR10 | EXTI_IMR_MR11 |
                    EXTI_IMR_MR12 | EXTI_IMR_MR13 | EXTI_IMR_MR14);
    NVIC_SetPriority(EXTI4_15_IRQn, 3);
    sensors_enable();
}

void EXTI4_15_IRQHandler() {
    uint32_t PR = EXTI->PR; // slight optimization for volatile

    if (PR & EXTI_PR_PR8) {
        sensors_scored(Score_10);
        EXTI -> PR = EXTI_PR_PR8;

    } else if (PR & EXTI_PR_PR9) {
        sensors_scored(Score_10);
        EXTI -> PR = EXTI_PR_PR9;

    } else if (PR & EXTI_PR_PR10) {
        sensors_scored(Score_20);
        EXTI -> PR = EXTI_PR_PR10;

    } else if (PR & EXTI_PR_PR11) {
        sensors_scored(Score_30);
        EXTI -> PR = EXTI_PR_PR11;

    } else if (PR & EXTI_PR_PR12) {
        sensors_scored(Score_50);
        EXTI -> PR = EXTI_PR_PR12;

    } else if (PR & EXTI_PR_PR13) {
        sensors_scored(Score_50);
        EXTI -> PR = EXTI_PR_PR13;

    } else if (PR & EXTI_PR_PR14) {
        sensors_scored(Score_Gutter);
        EXTI -> PR = EXTI_PR_PR14;

    }
}


