
#include "ws2812b.h"
#include "HUB75.h"
#include "HUB75_font.h"
#include "SoundLib.h"
#include "keypad.h"
#include "sensors.h"
#include "GameLogic.h"
#include "stm32f0xx.h"

const int tones[][2] = {
		{277, 350},
		{311, 415},
		{330, 392},
		{261, 311},
		{350, 440}
};

int lastKey;

#define SCORE_X 1
#define SCORE_Y 1

#define BALL_CNT_X 1
#define BALL_CNT_Y 9

#define NORMAL_BALL_COUNT 9
#define TIME_MAX_TIME (5*60)

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
	hub75_font_render_int_7x5(gameScore, 0, SCORE_X, SCORE_Y, 1, 1, 1, 0, 0, 0);
}

void render_ball_count() {
	hub75_font_render_int_7x5(gameBallCnt, 0, BALL_CNT_X, BALL_CNT_Y, 1, 1, 1, 0, 0, 0);
}

void render_timer() {
	int time = (gameEndTime - gameTime) / GAME_SECOND;

	hub75_font_render_7x5('0' + time / 60,         1, 9, 1, 1, 1, 0, 0, 0);
	hub75_font_render_7x5(':',                     7, 9, 1, 1, 1, 0, 0, 0);
	hub75_font_render_7x5('0' + (time % 60) / 10, 13, 9, 1, 1, 1, 0, 0, 0);
	hub75_font_render_7x5('0' + (time % 10),      19, 9, 1, 1, 1, 0, 0, 0);
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
	hub75_font_render_str_7x5("SELECT A", 1,  1, 1, 1, 1, 0, 0, 0);
	hub75_font_render_str_7x5("MODE",     1,  9, 1, 1, 1, 0, 0, 0);
	hub75_font_render_str_7x5("A B C D",  1, 17, 1, 1, 1, 0, 0, 0);
}

void render_confirmation() {
	render_back();
	hub75_font_render_str_7x5("CONFIRM",   1,  1, 1, 1, 1, 0, 0, 0);
	hub75_font_render_str_7x5("GAME MODE", 1,  9, 1, 1, 1, 0, 0, 0);
}

void render_time_input() {
	render_back();
	hub75_font_render_7x5(timeBuffer[2],  1, 1, 1, 1, 1, 0, 0, 0);
	hub75_font_render_7x5(':',            7, 1, 1, 1, 1, 0, 0, 0);
	hub75_font_render_7x5(timeBuffer[1], 13, 1, 1, 1, 1, 0, 0, 0);
	hub75_font_render_7x5(timeBuffer[0], 19, 1, 1, 1, 1, 0, 0, 0);
}

void render_final_score() {
	render_back();
	render_score();
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
}

void score_normal(ScoreType scoreType) {
	gameBallCnt--;
	if (gameBallCnt == 0) {
		gameState = Game_Final_Score;
	}

	switch (scoreType) {
	case Score_Gutter:
		break;
	case Score_10:
		gameScore += 10;
		break;
	case Score_20:
		gameScore += 20;
		break;
	case Score_30:
		gameScore += 30;
		break;
	case Score_50:
		gameScore += 50;
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
}

void score_time_attack(ScoreType scoreType) {
	switch (scoreType) {
	case Score_Gutter:
		break;
	case Score_10:
		gameScore += 10;
		break;
	case Score_20:
		gameScore += 20;
		break;
	case Score_30:
		gameScore += 30;
		break;
	case Score_50:
		gameScore += 50;
		break;
	}

	render_score();
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
		//score_streak(scoreType);
		break;
	case Mode_Combo:
		//score_combo(scoreType);
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
		default:
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
				render_back();
				render_score();
				gameState = Game_Playing;
				break;
			case Mode_Combo:
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
				start_time_attack();
				gameState = Game_Playing;
			}
			break;
		default:
			break;
		}
		break;
	case Game_Playing:
		break;
	case Game_Final_Score:
		gameState = Game_Mode_Select;
		gameMode  = Mode_None;
		render_mode_select();
		break;
	}

	lastKey = key;
}

void TIM1_BRK_UP_TRG_COM_IRQHandler() {
	TIM1->SR &= ~TIM_SR_UIF;

	gameTime++;

	if (gameMode == Mode_Time_Attack && gameState == Game_Playing) {
		// TODO render timer
		render_timer();

		if (gameTime == gameEndTime) {
			gameState = Game_Final_Score;
			render_final_score();
		}
	}
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

	// TIM1 for game tick

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->DIER  = TIM_DIER_UIE;
	TIM1->PSC   = 48000 - 1;
	TIM1->ARR   = 200 - 1;
	TIM1->CR1 |= TIM_CR1_CEN;
	NVIC->ISER[0] |= (1 << TIM1_BRK_UP_TRG_COM_IRQn);

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 3);

	// HUB75 initial test.
	render_mode_select();

	for(;;) {
		asm volatile ("wfi;");
	}
}
