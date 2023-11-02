#ifndef SOUND_LIB_H
#define SOUND_LIB_H

#include "stm32f0xx.h"

// Parameters for the synthesis rate.
#define N 1000
#define SOUND_RATE 20000
#define SOUND_SECOND SOUND_RATE
#define SOUND_N_CHANNELS 2
#define SOUND_MAX_VOL ((1<<12) / SOUND_N_CHANNELS)

// Sound modes
typedef enum {
	Sound_Single,
	Sound_Mixed
} SoundMode;

void sound_init(void);
void sound_set_freq(int chan, int f, int volume);
void sound_start_for(int len);
void sound_play(int freq1, int vol1, int freq2, int vol2, SoundMode mode, int len);
void sound_wait();

#endif
