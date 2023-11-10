#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define GAME_SECOND 5

typedef enum {
    Score_Gutter,
    Score_10,
    Score_20,
    Score_30,
    Score_50
} ScoreType;

typedef enum {
	Mode_None,
	Mode_Normal,
	Mode_Time_Attack,
	Mode_Streak,
	Mode_Combo
} GameModes;

typedef enum {
	Game_Mode_Select,
	Game_Inputing,
	Game_Playing,
	Game_Final_Score
} GameState;

extern GameModes gameMode;
extern GameState gameState;

extern uint32_t  gameScore;
extern ScoreType gameStreakType;
extern uint32_t  gameEndTime;
extern uint32_t  gameMulti;
extern uint32_t  gameTime;
extern uint32_t  gameBallCnt;

#endif
