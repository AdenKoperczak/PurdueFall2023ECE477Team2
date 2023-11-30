
#include "ws2812b.h"
#include "HUB75.h"
#include "HUB75_font.h"
#include "SoundLib.h"
#include "keypad.h"
#include "sensors.h"
#include "GameLogic.h"
#include "colors.h"
#include "stm32f0xx.h"

int lastKey;
int doSound;

#define NORMAL_BALL_COUNT 9
#define TIME_MAX_TIME (5*60)
#define MAX_SCORE 99999

#define COMBO_MULTI_DEN 2
#define COMBO_MULTI_INC 1
#define COMBO_MULTI_MAX ((int)(COMBO_MULTI_DEN * 5))


const uint8_t multi_colors[][3] = {
		{1, 0, 0},
		{1, 1, 0},
		{0, 1, 0}
};

GameModes gameMode;
GameState gameState;

uint32_t  gameScore;

uint32_t  gameEndTime;
uint32_t  gameTime;

ScoreType gameStreakType;
uint32_t  gameMulti;
uint32_t  gameBallCnt;

char timeBuffer[4];

void render_score() {
	if (gameScore > MAX_SCORE) {
		gameScore = MAX_SCORE;
	}
	hub75_font_render_str("SCORE", 1, 1, 1, 1, 1, 0, 0, 0);
	hub75_font_render_int(gameScore, 0, 34, 1, SCORE_COLOR, 0, 0, 0);
}

void render_ball_count() {
	hub75_font_render_str("BALLS", 1, 9, 1, 1, 1, 0, 0, 0);
	hub75_font_render_int(gameBallCnt, 0, 34, 9, BALL_COUNT_COLOR, 0, 0, 0);
}

void render_timer() {
	int time = (gameEndTime - gameTime) / GAME_SECOND;

	hub75_font_render('0' + time / 60,         1, 9, TIMER_COLOR, 0, 0, 0);
	hub75_font_render(':',                     6, 9, TIMER_COLOR, 0, 0, 0);
	hub75_font_render('0' + (time % 60) / 10, 11, 9, TIMER_COLOR, 0, 0, 0);
	hub75_font_render('0' + (time % 10),      17, 9, TIMER_COLOR, 0, 0, 0);

	if (time <= TIMER_FAST_FLASH) {
		lights_set_mode(L_Mode_Fast_Flash, TIMER_FAST_FLASH_COLOR);
	} else if (time <= TIMER_SLOW_FLASH) {
		lights_set_mode(L_Mode_Slow_Flash, TIMER_SLOW_FLASH_COLOR);
	}
}

void render_back() {
	for (int x = 0; x < HUB75_WIDTH; x++) {
		for (int y = 0; y < HUB75_HIEGHT; y++) {
			hub75_set_pixel(x, y, 0, 0, 0);
		}
	}
}

void render_mode_select() {
	render_back();
	hub75_font_render_str("SELECT A", 1,  1, 1, 1, 1, 0, 0, 0);
	hub75_font_render_str("MODE",     1,  9, 1, 1, 1, 0, 0, 0);
	hub75_font_render('A',  1, 17, MODE_NORMAL_COLOR,      0, 0, 0);
	hub75_font_render('B', 10, 17, MODE_TIME_ATTACK_COLOR, 0, 0, 0);
	hub75_font_render('C', 19, 17, MODE_STREAK_COLOR,      0, 0, 0);
	hub75_font_render('D', 28, 17, MODE_COMBO_COLOR,       0, 0, 0);

	if (!doSound) {
		hub75_font_render('N', 58, 23, 1, 1, 1, 0, 0, 0);
	}

	lights_set_mode(L_Mode_Select, 0, 0, 0);
}

void render_confirmation() {
	render_back();
	hub75_font_render_str("PLAY", 1, 1, 1, 1, 1, 0, 0, 0);
	switch (gameMode) {
	case Mode_Normal:
		hub75_font_render_str("NORMAL", 1, 9, MODE_NORMAL_COLOR, 0, 0, 0);
		lights_set_mode(L_Mode_Solid, MODE_NORMAL_COLOR);
		break;
	case Mode_Time_Attack:
		hub75_font_render_str("TIME",    1, 9, MODE_TIME_ATTACK_COLOR, 0, 0, 0);
		hub75_font_render_str("ATTACK", 28, 9, MODE_TIME_ATTACK_COLOR, 0, 0, 0);
		lights_set_mode(L_Mode_Solid, MODE_TIME_ATTACK_COLOR);
		break;
	case Mode_Streak:
		hub75_font_render_str("STREAK", 1, 9, MODE_STREAK_COLOR, 0, 0, 0);
		lights_set_mode(L_Mode_Solid, MODE_STREAK_COLOR);
		break;
	case Mode_Combo:
		hub75_font_render_str("COMBO", 1, 9, MODE_COMBO_COLOR, 0, 0, 0);
		lights_set_mode(L_Mode_Solid, MODE_COMBO_COLOR);
		break;
	default:
		hub75_font_render_str("INVALID", 1, 9, 1, 1, 1, 0, 0, 0);
		lights_set_mode(L_Mode_Slow_Flash, 0xff, 0xff, 0xff);
		break;
	}

	hub75_font_render_str("MODE", 1, 17, 1, 1, 1, 0, 0, 0);

}

void render_time_input() {
	hub75_font_render_str("ENTER TIME", 1, 1, 1, 1, 1, 0, 0, 0);

	hub75_font_render(timeBuffer[2],  1, 9, 1, 1, 1, 0, 0, 0);
	hub75_font_render(':',            6, 9, 1, 1, 1, 0, 0, 0);
	hub75_font_render(timeBuffer[1], 11, 9, 1, 1, 1, 0, 0, 0);
	hub75_font_render(timeBuffer[0], 17, 9, 1, 1, 1, 0, 0, 0);
}

void render_final_score() {
	render_back();
	render_score();
	hub75_font_render_str("GAME OVER", 1,  9, GAME_OVER_COLOR, 0, 0, 0);

	lights_set_mode(L_Mode_Slow_Flash, GAME_OVER_COLOR);
}


void render_multi() {

	switch (gameStreakType) {
	case Score_10:
		hub75_font_render_str(" 10X", 1, 17, 1, 1, 1, 0, 0, 0);
		break;
	case Score_20:
		hub75_font_render_str(" 20X", 1, 17, 1, 1, 1, 0, 0, 0);
		break;
	case Score_30:
		hub75_font_render_str(" 30X", 1, 17, 1, 1, 1, 0, 0, 0);
		break;
	case Score_50:
		hub75_font_render_str("500X", 1, 17, 1, 1, 1, 0, 0, 0);
		break;
	case Score_Gutter:
		hub75_font_render_str("  0X", 1, 17, 1, 1, 1, 0, 0, 0);
		break;
	}
	int whole = gameMulti / COMBO_MULTI_DEN;
	int frac  = ((gameMulti * 10) / COMBO_MULTI_DEN) % 10;


	hub75_font_render('.',            30, 17, 1, 1, 1, 0, 0, 0); // rendered first because it overlaps
	hub75_font_render_int_n(whole, 1, 25, 17, 1, 1, 1, 0, 0, 0);
	hub75_font_render_int_n(frac,  1, 35, 17, 1, 1, 1, 0, 0, 0);

	for (int i = MULTI_BAR_LEFT; i < MULTI_BAR_RIGHT; i++) {
		if (i * (COMBO_MULTI_MAX - COMBO_MULTI_DEN) < (gameMulti - COMBO_MULTI_DEN) * HUB75_WIDTH) {
			int index = (i - MULTI_BAR_LEFT) * sizeof(multi_colors) / sizeof(multi_colors[0]) / (MULTI_BAR_RIGHT - MULTI_BAR_LEFT);
			hub75_set_pixel(i, HUB75_HIEGHT - 2, multi_colors[index][0], multi_colors[index][1], multi_colors[index][2]);
			hub75_set_pixel(i, HUB75_HIEGHT - 3, multi_colors[index][0], multi_colors[index][1], multi_colors[index][2]);
		} else {
			hub75_set_pixel(i, HUB75_HIEGHT - 2, 0, 0, 0);
			hub75_set_pixel(i, HUB75_HIEGHT - 3, 0, 0, 0);
		}

		hub75_set_pixel(i, HUB75_HIEGHT - 4, 1, 1, 1);
		hub75_set_pixel(i, HUB75_HIEGHT - 1, 1, 1, 1);
	}
}

int parse_time() {
	int total = 0;

	total += (timeBuffer[0] - '0') * 1;
	total += (timeBuffer[1] - '0') * 10;
	total += (timeBuffer[2] - '0') * 60;
	total += (timeBuffer[3] - '0') * 600;

	return total;
}

void update_input_time_back() {
	timeBuffer[0] = timeBuffer[1];
	timeBuffer[1] = timeBuffer[2];
	timeBuffer[2] = timeBuffer[3];
	timeBuffer[3] = '0';
}

void update_input_time(char c) {
	timeBuffer[3] = timeBuffer[2];
	timeBuffer[2] = timeBuffer[1];
	timeBuffer[1] = timeBuffer[0];
	timeBuffer[0] = c;

	int time = parse_time();

	if (time > TIME_MAX_TIME) {
		update_input_time_back();
	}
}


void start_normal() {
	gameBallCnt = NORMAL_BALL_COUNT;
	gameScore   = 0;

	render_back();
	render_score();
	render_ball_count();
	lights_set_mode(L_Mode_Chase, MODE_NORMAL_COLOR);
}

void score_normal(ScoreType scoreType) {
	gameBallCnt--;
	if (gameBallCnt == 0) {
		gameState = Game_Final_Score;
	}

	switch (scoreType) {
	case Score_Gutter:
		sound_gutter();
		break;
	case Score_10:
		sound_10();
		gameScore += 10;
		break;
	case Score_20:
		sound_20();
		gameScore += 20;
		break;
	case Score_30:
		sound_30();
		gameScore += 30;
		break;
	case Score_50:
		sound_50();
		gameScore += 500;
		break;
	}

	if (gameState == Game_Final_Score) {
		render_final_score();
	} else {
		render_ball_count();
		render_score();
	}
}

void start_time_attack() {
	gameEndTime = gameTime + parse_time() * GAME_SECOND;
	gameScore   = 0;

	render_back();
	render_score();
	lights_set_mode(L_Mode_Chase, MODE_TIME_ATTACK_COLOR);
}

void score_time_attack(ScoreType scoreType) {
	switch (scoreType) {
	case Score_Gutter:
		sound_gutter();
		break;
	case Score_10:
		sound_10();
		gameScore += 10;
		break;
	case Score_20:
		sound_20();
		gameScore += 20;
		break;
	case Score_30:
		sound_30();
		gameScore += 30;
		break;
	case Score_50:
		sound_50();
		gameScore += 500;
		break;
	}

	render_score();
}

void start_streak() {
	gameScore = 0;

	render_back();
	render_score();
	lights_set_mode(L_Mode_Chase, MODE_STREAK_COLOR);
}

void score_streak(ScoreType scoreType) {
	switch (scoreType) {
	case Score_Gutter:
	case Score_10:
		sound_gutter();
		gameState = Game_Final_Score;
		break;
	case Score_20:
		sound_20();
		gameScore += 20;
		break;
	case Score_30:
		sound_30();
		gameScore += 30;
		break;
	case Score_50:
		sound_50();
		gameScore += 500;
		break;
	}

	if (gameState == Game_Final_Score) {
		render_final_score();
	} else {
		render_score();
	}

}

void start_combo() {
	gameScore = 0;
	gameMulti = COMBO_MULTI_DEN;
	gameStreakType = Score_Gutter;
	gameEndTime = gameTime + parse_time() * GAME_SECOND;

	render_back();
	render_score();
	render_multi();
	render_timer();
	lights_set_mode(L_Mode_Chase, MODE_COMBO_COLOR);
}

void score_combo(ScoreType scoreType) {
	if (scoreType == gameStreakType && scoreType != Score_Gutter) {
		gameMulti += COMBO_MULTI_INC;
		if (gameMulti > COMBO_MULTI_MAX) {
			gameMulti = COMBO_MULTI_MAX;
		}
	} else {
		gameMulti = COMBO_MULTI_DEN;
		gameStreakType = scoreType;
	}


	switch (scoreType) {
	case Score_Gutter:
		sound_gutter();
		break;
	case Score_10:
		sound_10();
		gameScore += 10 / COMBO_MULTI_DEN * gameMulti;
		break;
	case Score_20:
		sound_20();
		gameScore += 20 / COMBO_MULTI_DEN * gameMulti;
		break;
	case Score_30:
		sound_30()
		gameScore += 30 / COMBO_MULTI_DEN * gameMulti;
		break;
	case Score_50:
		sound_50();
		gameScore += 500 / COMBO_MULTI_DEN * gameMulti;
		break;
	}

	render_score();
	render_multi();
}

void sensors_scored (ScoreType scoreType) {
	if (gameState != Game_Playing) {
		return;
	}
	// actual scoring logic
	switch (gameMode) {
	case Mode_Normal:
		score_normal(scoreType);
		break;
	case Mode_Time_Attack:
		score_time_attack(scoreType);
		break;
	case Mode_Streak:
		score_streak(scoreType);
		break;
	case Mode_Combo:
		score_combo(scoreType);
		break;
	case Mode_None:
	default:
		break;
	}
}

void keypad_callback(int key) {

	if (key == lastKey || lastKey != Keypad_None || key == Keypad_None || key == Keypad_Invl) {
		lastKey = key;
		return;
	}

	key = keypadChars[key];

	switch (gameState) {
	case Game_Mode_Select:
		switch (key) {
		case 'A':
			gameMode  = Mode_Normal;
			break;
		case 'B':
			gameMode  = Mode_Time_Attack;
			break;
		case 'C':
			gameMode  = Mode_Streak;
			break;
		case 'D':
			gameMode  = Mode_Combo;
			break;
		case '1':
			doSound ^= 1;
			render_mode_select();
			lastKey = key;
			return;
		default:
			lastKey = key;
			return;
		}
		gameState = Game_Mode_Confirm;
		render_confirmation();
		break;
	case Game_Mode_Confirm:
		if (key == '#') {
			switch(gameMode) {
			case Mode_Normal:
				start_normal();
				gameState = Game_Playing;
				break;
			case Mode_Time_Attack:
				render_back();
				render_time_input();
				gameState = Game_Inputing;
				break;
			case Mode_Streak:
				start_streak();
				gameState = Game_Playing;
				break;
			case Mode_Combo:
				render_back();
				render_time_input();
				gameState = Game_Inputing;
				break;
			default:
				render_mode_select();
				gameState = Game_Mode_Select;
				break;
			}
		} else if (key == '*') {
			render_mode_select();
			gameState = Game_Mode_Select;
		}
		break;
	case Game_Inputing:
		switch (key) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			update_input_time(key);
			render_time_input();
			break;
		case '*':
			update_input_time_back();
			render_time_input();
			break;
		case '#':
			if (parse_time() > 0) {
				switch (gameMode) {
				case Mode_Time_Attack:
					start_time_attack();
					break;
				case Mode_Combo:
					start_combo();
					break;
				default:
					break;
				}

				gameState = Game_Playing;
			}
			break;
		default:
			break;
		}
		break;
	case Game_Playing:
		if (key == '*') {
			gameState = Game_Final_Score;
			render_final_score();
		}
		break;
	case Game_Final_Score:
		gameState = Game_Mode_Select;
		gameMode  = Mode_None;
		render_mode_select();
		break;
	}

	lastKey = key;
}


int lightsPos;
int lightsColor;

void TIM1_BRK_UP_TRG_COM_IRQHandler() {
	TIM1->SR &= ~TIM_SR_UIF;

	gameTime++;

	if ((gameMode == Mode_Time_Attack || gameMode == Mode_Combo) && gameState == Game_Playing) {
		render_timer();

		if (gameTime == gameEndTime) {
			gameState = Game_Final_Score;
			render_final_score();
		}
	}

	lights_tick();
}

int main(void) {
	gameMode       = Mode_None;
	gameState      = Game_Mode_Select;

	gameScore      = 0;

	gameEndTime    = 0;
	gameTime       = 0;

	gameStreakType = Score_Gutter;
	gameMulti      = 5;
	gameBallCnt    = 1000;

	lightsPos  = 0;
	lightsColor = 1;
	doSound = 1;

	lastKey  = Keypad_None;

	timeBuffer[0] = '0';
	timeBuffer[1] = '0';
	timeBuffer[2] = '0';
	timeBuffer[3] = '0';

	ws2812b_init();
	hub75_init(30);
	sound_init();
	keypad_init();
	sensors_init();

	lights_set_mode(L_Mode_Off, 0, 0, 0);

	// TIM1 for game tick

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->DIER  = TIM_DIER_UIE;
	TIM1->PSC   = 48000 / 4 - 1;
	TIM1->ARR   = 125 - 1;
	TIM1->CR1 |= TIM_CR1_CEN;
	NVIC->ISER[0] |= (1 << TIM1_BRK_UP_TRG_COM_IRQn);

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 3);

	// HUB75 initial test.
	render_mode_select();

	for(;;) {
		asm volatile ("wfi;");
	}
}
