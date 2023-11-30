
#include "stm32f0xx.h"
#include "HUB75_font.h"

const Hub75Char8x6 char8x6Sspace = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const Hub75Char8x6 char8x6N0 = {0x3f, 0x21, 0x21, 0x2d, 0x2d, 0x21, 0x21, 0x3f};
const Hub75Char8x6 char8x6N1 = {0x04, 0x0c, 0x14, 0x04, 0x04, 0x04, 0x04, 0x04};


const Hub75Char8x6 * hub75_font_get_8x6(char c) {

	switch (c) {
		case '0': return &char8x6N0;
		case '1': return &char8x6N1;
		case ' ': return &char8x6Sspace;

	}

	return &char8x6Sspace;
}

void hub75_font_render_from_8x6(const Hub75Char8x6 * c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	for (int x = 0; x < 6; x++) {
		for (int y = 0; y < 8; y++) {
			if ((*c)[y] & (1 << (5 - x))) {
				hub75_set_pixel(xp + x, yp + y, fr, fg, fb);
			} else {
				hub75_set_pixel(xp + x, yp + y, br, bg, bb);
			}
		}
	}
}

void hub75_font_render_8x6(char c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	hub75_font_render_from_8x6(hub75_font_get_8x6(c), xp, yp, fr, fg, fb, br, bg, bb);
}

const Hub75Char7x5 char7x5N0 = {0x0e, 0x11, 0x11, 0x15, 0x11, 0x11, 0x0e};
const Hub75Char7x5 char7x5N1 = {0x04, 0x0c, 0x14, 0x04, 0x04, 0x04, 0x1f};
const Hub75Char7x5 char7x5N2 = {0x0e, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1f};
const Hub75Char7x5 char7x5N3 = {0x0e, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0e};
const Hub75Char7x5 char7x5N4 = {0x11, 0x11, 0x11, 0x1f, 0x01, 0x01, 0x01};
const Hub75Char7x5 char7x5N5 = {0x1f, 0x10, 0x10, 0x1e, 0x01, 0x01, 0x1e};
const Hub75Char7x5 char7x5N6 = {0x0e, 0x10, 0x10, 0x1e, 0x11, 0x11, 0x0e};
const Hub75Char7x5 char7x5N7 = {0x1e, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04};
const Hub75Char7x5 char7x5N8 = {0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e};
const Hub75Char7x5 char7x5N9 = {0x0e, 0x11, 0x11, 0x0f, 0x01, 0x01, 0x01};
const Hub75Char7x5 char7x5Sasterisk = {0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x00};
const Hub75Char7x5 char7x5Sbar = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
const Hub75Char7x5 char7x5Scolon = {0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00};
const Hub75Char7x5 char7x5Sdash = {0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00};
const Hub75Char7x5 char7x5Speriod = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04};
const Hub75Char7x5 char7x5Spound = {0x0a, 0x0a, 0x1f, 0x0a, 0x1f, 0x0a, 0x0a};
const Hub75Char7x5 char7x5Sspace = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const Hub75Char7x5 char7x5Ua = {0x04, 0x0a, 0x11, 0x11, 0x1f, 0x11, 0x11};
const Hub75Char7x5 char7x5Ub = {0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e};
const Hub75Char7x5 char7x5Uc = {0x07, 0x08, 0x10, 0x10, 0x10, 0x08, 0x07};
const Hub75Char7x5 char7x5Ud = {0x1c, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1c};
const Hub75Char7x5 char7x5Ue = {0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f};
const Hub75Char7x5 char7x5Uf = {0x1f, 0x10, 0x10, 0x1c, 0x10, 0x10, 0x10};
const Hub75Char7x5 char7x5Ug = {0x0e, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0e};
const Hub75Char7x5 char7x5Uh = {0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11};
const Hub75Char7x5 char7x5Ui = {0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1f};
const Hub75Char7x5 char7x5Uj = {0x1f, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0c};
const Hub75Char7x5 char7x5Uk = {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11};
const Hub75Char7x5 char7x5Ul = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f};
const Hub75Char7x5 char7x5Um = {0x11, 0x1b, 0x15, 0x11, 0x11, 0x11, 0x11};
const Hub75Char7x5 char7x5Un = {0x11, 0x19, 0x19, 0x15, 0x13, 0x13, 0x11};
const Hub75Char7x5 char7x5Uo = {0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e};
const Hub75Char7x5 char7x5Up = {0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10};
const Hub75Char7x5 char7x5Uq = {0x1e, 0x12, 0x12, 0x12, 0x16, 0x13, 0x1e};
const Hub75Char7x5 char7x5Ur = {0x1e, 0x11, 0x11, 0x1e, 0x14, 0x12, 0x11};
const Hub75Char7x5 char7x5Us = {0x0f, 0x10, 0x10, 0x0e, 0x01, 0x01, 0x1e};
const Hub75Char7x5 char7x5Ut = {0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
const Hub75Char7x5 char7x5Uu = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e};
const Hub75Char7x5 char7x5Uv = {0x11, 0x11, 0x11, 0x0a, 0x0a, 0x04, 0x04};
const Hub75Char7x5 char7x5Uw = {0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0a};
const Hub75Char7x5 char7x5Ux = {0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11};
const Hub75Char7x5 char7x5Uy = {0x11, 0x11, 0x0a, 0x0a, 0x04, 0x04, 0x04};
const Hub75Char7x5 char7x5Uz = {0x1f, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1f};

const Hub75Char7x5 * hub75_font_get(char c) {

	switch (c) {
		case '0': return &char7x5N0;
		case '1': return &char7x5N1;
		case '2': return &char7x5N2;
		case '3': return &char7x5N3;
		case '4': return &char7x5N4;
		case '5': return &char7x5N5;
		case '6': return &char7x5N6;
		case '7': return &char7x5N7;
		case '8': return &char7x5N8;
		case '9': return &char7x5N9;
		case '*': return &char7x5Sasterisk;
		case '|': return &char7x5Sbar;
		case ':': return &char7x5Scolon;
		case '-': return &char7x5Sdash;
		case '.': return &char7x5Speriod;
		case '#': return &char7x5Spound;
		case ' ': return &char7x5Sspace;
		case 'A': return &char7x5Ua;
		case 'B': return &char7x5Ub;
		case 'C': return &char7x5Uc;
		case 'D': return &char7x5Ud;
		case 'E': return &char7x5Ue;
		case 'F': return &char7x5Uf;
		case 'G': return &char7x5Ug;
		case 'H': return &char7x5Uh;
		case 'I': return &char7x5Ui;
		case 'J': return &char7x5Uj;
		case 'K': return &char7x5Uk;
		case 'L': return &char7x5Ul;
		case 'M': return &char7x5Um;
		case 'N': return &char7x5Un;
		case 'O': return &char7x5Uo;
		case 'P': return &char7x5Up;
		case 'Q': return &char7x5Uq;
		case 'R': return &char7x5Ur;
		case 'S': return &char7x5Us;
		case 'T': return &char7x5Ut;
		case 'U': return &char7x5Uu;
		case 'V': return &char7x5Uv;
		case 'W': return &char7x5Uw;
		case 'X': return &char7x5Ux;
		case 'Y': return &char7x5Uy;
		case 'Z': return &char7x5Uz;
	}

	return &char7x5Sspace;
}

void hub75_font_render_from(const Hub75Char7x5 * c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 7; y++) {
			if ((*c)[y] & (1 << (4 - x))) {
				hub75_set_pixel(xp + x, yp + y, fr, fg, fb);
			} else {
				hub75_set_pixel(xp + x, yp + y, br, bg, bb);
			}
		}
	}
}

void hub75_font_render(char c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	hub75_font_render_from(hub75_font_get(c), xp, yp, fr, fg, fb, br, bg, bb);
}

void hub75_font_render_str(char * c, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	while (*c != '\0') {
		hub75_font_render(*c, xp, yp, fr, fg, fb, br, bg, bb);

		c++;
		xp += 6;
	}
}

void hub75_font_render_int(int i, int dir, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	if (!dir) {
		int iCopy = i;
		int n = -1;
		do {
			iCopy /= 10;
			n++;
		} while (iCopy > 0);

		xp += n * 6;
	}

	do {
		hub75_font_render('0' + (i % 10), xp, yp, fr, fg, fb, br, bg, bb);
		xp -= 6;
		i /= 10;
	} while (i > 0 && xp >= 0 && xp < HUB75_WIDTH);
}

void hub75_font_render_int_n(int i, int n, int xp, int yp, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {

	xp += 6 * (n - 1);

	for (;n > 0; n--) {
		hub75_font_render('0' + (i % 10), xp, yp, fr, fg, fb, br, bg, bb);
		xp -= 6;
		i /= 10;
	}
}














