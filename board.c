#include <stdio.h>
#include "board.h"

void initialize_board(char board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            board[r][c] = EMPTY;
}

void display_board(char board[ROWS][COLS]) {
    puts("");
    for (int r = 0; r < ROWS; ++r) {
        printf("|");
        for (int c = 0; c < COLS; ++c)
            printf(" %c", board[r][c]);
        puts(" |");
    }
    puts("  1 2 3 4 5 6 7");
    puts("");
}

int is_valid_column(char board[ROWS][COLS], int col) {
    return col >= 0 && col < COLS && board[0][col] == EMPTY;
}

int drop_disc(char board[ROWS][COLS], int col, char disc) {
    if (!is_valid_column(board, col)) return 0;

    for (int r = ROWS - 1; r >= 0; --r) {
        if (board[r][col] == EMPTY) {
            board[r][col] = disc;
            return 1;
        }
    }
    return 0;
}

int has_won(char board[ROWS][COLS], char disc) {
    const int dr[] = {0, 1, 1, 1};
    const int dc[] = {1, 0, 1, -1};

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (board[r][c] != disc) continue;

            for (int d = 0; d < 4; ++d) {
                int end_r = r + 3 * dr[d];
                int end_c = c + 3 * dc[d];

                if (end_r < 0 || end_r >= ROWS ||
                    end_c < 0 || end_c >= COLS) continue;

                int match = 1;
                for (int k = 1; k < 4; ++k) {
                    if (board[r + k * dr[d]][c + k * dc[d]] != disc) {
                        match = 0;
                        break;
                    }
                }
                if (match) return 1;
            }
        }
    }
    return 0;
}

int is_board_full(char board[ROWS][COLS]) {
    for (int c = 0; c < COLS; ++c)
        if (board[0][c] == EMPTY) return 0;
    return 1;
}
