#include "SoundLib.h"
#include "stm32f0xx.h"

int main(void) {
    sound_init();

#define SOUND
//#define TEST

#ifdef SOUND
    int playtime = 0.5 * SOUND_SECOND;
    int vol      = SOUND_MAX_VOL;

    for (;;) {
        sound_play(277, vol, 350, vol, Sound_Mixed, playtime);
        sound_wait();
        sound_play(0, 0, 0, 0, Sound_Single, playtime);
        sound_wait();

        sound_play(311, vol, 415, vol, Sound_Mixed, playtime);
        sound_wait();
        sound_play(0, 0, 0, 0, Sound_Single, playtime);
        sound_wait();

        sound_play(330, vol, 392, vol, Sound_Mixed, playtime);
        sound_wait();
        sound_play(0, 0, 0, 0, Sound_Single, playtime);
        sound_wait();

        sound_play(261, vol, 311, vol, Sound_Mixed, playtime);
        sound_wait();
        sound_play(0, 0, 0, 0, Sound_Single, playtime);
        sound_wait();

        sound_play(350, vol, 440, vol, Sound_Mixed, playtime);
        sound_wait();
        sound_play(0, 0, 0, 0, Sound_Single, playtime);
        sound_wait();

    }
#endif
#ifdef TEST
    int playtime = 3600 * SOUND_SECOND;
    int vol = SOUND_MAX_VOL * 0.8;

    for (;;) {
		sound_play(440, vol, 0, 0, 1, playtime);
		sound_wait();
    }

#endif

}

