#ifndef AI_H
#define AI_H

#include "board.h"

enum Difficulty {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3
};

int choose_ai_move(char board[ROWS][COLS], enum Difficulty difficulty);

#endif
