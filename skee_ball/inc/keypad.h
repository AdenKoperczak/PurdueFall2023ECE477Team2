#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_CLOCK_DIV 0
#define KEYPAD_CLOCK_PRE (4800 - 1)
#define KEYPAD_CLOCK_ARR (20 - 1)

#define Keypad_None -1
#define Keypad_Invl -2

extern volatile int keypadKey;
extern const char keypadChars[16];

void keypad_init();
void keypad_callback();

#endif
