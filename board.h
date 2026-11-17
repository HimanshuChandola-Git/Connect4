#ifndef BOARD_H
#define BOARD_H

#define ROWS 6
#define COLS 7
#define EMPTY '.'
#define PLAYER1 'X'
#define PLAYER2 'O'

void initialize_board(char board[ROWS][COLS]);
void display_board(char board[ROWS][COLS]);
int drop_disc(char board[ROWS][COLS], int col, char disc);
int is_valid_column(char board[ROWS][COLS], int col);
int has_won(char board[ROWS][COLS], char disc);
int is_board_full(char board[ROWS][COLS]);

#endif
