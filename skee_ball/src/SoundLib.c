#include "SoundLib.h"
#include "stm32f0xx.h"

typedef struct {
	volatile int step;
	volatile int offset;
	volatile int volume;
} Channel;

volatile Channel channels[SOUND_N_CHANNELS];

volatile int samp;
volatile int playing;

volatile uint32_t currentTime;
volatile uint32_t offTime;




void enable_dac(void) {
    DAC->CR |= DAC_CR_EN1;
    playing = 1;
}

void disable_dac(void) {
    DAC->CR &= ~DAC_CR_EN1;
    playing = 0;
}

//============================================================================
// Timer 6 ISR
//============================================================================
// Write the Timer 6 ISR here.  Be sure to give it the right name.

void TIM6_DAC_IRQHandler() {
    TIM6->SR &= ~TIM_SR_UIF;
    samp = 0;

    for (int i = 0; i < SOUND_N_CHANNELS; i++) {
    	channels[i].offset += channels[i].step;
    	if (channels[i].offset >= channels[i].volume) {
    		channels[i].offset = 0;
    	}

    	samp += channels[i].offset;
    }
    DAC->DHR12R1 = samp;

    currentTime += 1;

    if (currentTime == offTime) {
    	disable_dac();
    }
}

//============================================================================
// sound_init()
//============================================================================


void sound_init(void) {
	// initialize variables;
	currentTime = 0;
	offTime     = 0;
	playing 	= 0;
	samp        = 0;

	for (int i = 0; i < SOUND_N_CHANNELS; i++) {
		channels[i].offset = 0;
		channels[i].step   = 0;
		channels[i].volume = 0;
	}


	// initialize GPIO/DAC
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= 0xfffffcff;
    GPIOA->MODER |= 0x00000300;
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    DAC->CR &= 0xffffffc7;
    DAC->CR |= 0x00000005;

    // initialize tim6
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 480 - 1;
    TIM6->ARR = 100000 / SOUND_RATE - 1;
    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR2 &= 0xfffffff8;
    TIM6->CR2 |= 0x00000020;
    TIM6->CR1 |= 0x00000001;
    NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;
    NVIC_SetPriority(TIM6_DAC_IRQn, 1);
}

//============================================================================
// sound_set_freq()
//============================================================================
void sound_set_freq(int chan, int f, int volume) {
    channels[chan].volume = volume;

    channels[chan].offset = 0;
    channels[chan].step = (f * volume / SOUND_RATE);
}


//============================================================================
// sound_start_for()
//
// Parameters: len: length sound will be played for in samples. Use SOUND_SECONDS
//  				times your wanted time in seconds to get len.
//============================================================================
void sound_start_for(int len) {
	playing = 1;

    offTime = currentTime + len;
    enable_dac();
}

//============================================================================
// sound_play()
//
// Parameters: freq1: Digital Channel 1 frequency
//  		   vol1:  Channel 1 Volume
//             freq2: Digital Channel 2 frequency
//  		   vol1:  Channel 2 Volume
//             mode: Sound_Single/Sound_Mixed default single tone
//             len: length sound will be played in samples
//============================================================================
// Frequency combinations:
// (277, 350), (311, 415), (330, 392), (261, 311)
void sound_play(int freq1, int vol1, int freq2, int vol2, SoundMode mode, int len) {
	// good enough sync
	if (playing) {
		return;
	}
	playing = 1;

    samp = 0;
    if (mode == Sound_Mixed) {
        sound_set_freq(0, freq1, vol1);
        sound_set_freq(1, freq2, vol2);
    } else {
        sound_set_freq(0, freq1, vol1);
        sound_set_freq(1, freq1, vol1);
    }

    sound_start_for(len);
}

//============================================================================
// sound_wait()
//
// Wait for sound to finish. Will not be used in final code.
//============================================================================

void sound_wait() {
    while (playing) asm volatile ("wfi;");
}
