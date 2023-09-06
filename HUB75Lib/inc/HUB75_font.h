#ifndef HUB75_FONT_H
#define HUB75_FONT_H

#include "stm32f0xx.h"
#include "HUB75.h"

typedef uint8_t Hub75Char8x6[8];

Hub75Char8x6 * hub75_font_get_8x6(char c);
void hub75_font_render_from_8x6(Hub75Pix * pixels, Hub75Char8x6 * c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb);
void hub75_font_render_8x6(Hub75Pix * pixels, char c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb);

#endif
