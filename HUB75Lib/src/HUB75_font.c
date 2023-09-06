
#include "stm32f0xx.h"
#include "HUB75_font.h"

Hub75Char8x6 char8x6Sspace = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

Hub75Char8x6 char8x6N0 = {
		0x3f,
		0x21,
		0x21,
		0x2d,
		0x2d,
		0x21,
		0x21,
		0x3f
};

Hub75Char8x6 char8x6N1 = {
		0x04,
		0x0c,
		0x14,
		0x04,
		0x04,
		0x04,
		0x04,
		0x04
};




Hub75Char8x6 * hub75_font_get_8x6(char c) {

	switch (c) {
		case '0': return &char8x6N0;
		case '1': return &char8x6N1;
		case ' ': return &char8x6Sspace;

	}

	//return &char8x6N1;
	return &char8x6Sspace;
}

void hub75_font_render_from_8x6(Hub75Pix * pixels, Hub75Char8x6 * c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	for (int x = 0; x < 6; x++) {
		for (int y = 0; y < 8; y++) {
			if ((*c)[y] & (1<<x)) {
				hub75_set_pixel(pixels, xp + x, yp + y, fr, fg, fb);
			} else {
				hub75_set_pixel(pixels, xp + x, yp + y, br, bg, bb);
			}
		}
	}
}

void hub75_font_render_8x6(Hub75Pix * pixels, char c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	hub75_font_render_from_8x6(pixels, hub75_font_get_8x6(c), xp, yp, fr, fg, fb, br, bg, bb);
}
