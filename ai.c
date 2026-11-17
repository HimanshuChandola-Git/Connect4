#include <stdlib.h>
#include <limits.h>
#include "ai.h"

#define WIN_SCORE 1000000
#define HARD_DEPTH 7
#define MEDIUM_DEPTH 4

static int evaluate_window(const char window[4]) {
    int ai = 0, human = 0, empty = 0;
    for (int i = 0; i < 4; ++i) {
        if (window[i] == PLAYER2) ++ai;
        else if (window[i] == PLAYER1) ++human;
        else ++empty;
    }

    if (ai == 4) return 100000;
    if (human == 4) return -100000;
    if (ai == 3 && empty == 1) return 120;
    if (ai == 2 && empty == 2) return 12;
    if (human == 3 && empty == 1) return -150;
    if (human == 2 && empty == 2) return -10;
    return 0;
}

static int evaluate_board(char board[ROWS][COLS]) {
    int score = 0;
    char window[4];

   
    for (int r = 0; r < ROWS; ++r) {
        if (board[r][COLS / 2] == PLAYER2) score += 6;
        else if (board[r][COLS / 2] == PLAYER1) score -= 6;
    }

    
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c <= COLS - 4; ++c) {
            for (int k = 0; k < 4; ++k) window[k] = board[r][c + k];
            score += evaluate_window(window);
        }

    
    for (int r = 0; r <= ROWS - 4; ++r)
        for (int c = 0; c < COLS; ++c) {
            for (int k = 0; k < 4; ++k) window[k] = board[r + k][c];
            score += evaluate_window(window);
        }

    
    for (int r = 0; r <= ROWS - 4; ++r)
        for (int c = 0; c <= COLS - 4; ++c) {
            for (int k = 0; k < 4; ++k) window[k] = board[r + k][c + k];
            score += evaluate_window(window);
        }

    
    for (int r = 3; r < ROWS; ++r)
        for (int c = 0; c <= COLS - 4; ++c) {
            for (int k = 0; k < 4; ++k) window[k] = board[r - k][c + k];
            score += evaluate_window(window);
        }

    return score;
}

static int get_valid_moves(char board[ROWS][COLS], int moves[COLS]) {
    const int order[COLS] = {3, 2, 4, 1, 5, 0, 6};
    int count = 0;
    for (int i = 0; i < COLS; ++i)
        if (is_valid_column(board, order[i]))
            moves[count++] = order[i];
    return count;
}

static int minimax(char board[ROWS][COLS], int depth,
                   int alpha, int beta, int maximizing) {
    if (has_won(board, PLAYER2)) return WIN_SCORE + depth;
    if (has_won(board, PLAYER1)) return -WIN_SCORE - depth;
    if (is_board_full(board) || depth == 0) return evaluate_board(board);

    int moves[COLS];
    int count = get_valid_moves(board, moves);

    if (maximizing) {
        int best = INT_MIN;
        for (int i = 0; i < count; ++i) {
            int col = moves[i];
            drop_disc(board, col, PLAYER2);
            int value = minimax(board, depth - 1, alpha, beta, 0);
            for (int r = 0; r < ROWS; ++r)
                if (board[r][col] == PLAYER2) {
                    board[r][col] = EMPTY;
                    break;
                }
            if (value > best) best = value;
            if (best > alpha) alpha = best;
            if (alpha >= beta) break;
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < count; ++i) {
            int col = moves[i];
            drop_disc(board, col, PLAYER1);
            int value = minimax(board, depth - 1, alpha, beta, 1);
            for (int r = 0; r < ROWS; ++r)
                if (board[r][col] == PLAYER1) {
                    board[r][col] = EMPTY;
                    break;
                }
            if (value < best) best = value;
            if (best < beta) beta = best;
            if (alpha >= beta) break;
        }
        return best;
    }
}

static int immediate_winning_move(char board[ROWS][COLS], char disc) {
    for (int c = 0; c < COLS; ++c) {
        if (!is_valid_column(board, c)) continue;
        drop_disc(board, c, disc);
        int wins = has_won(board, disc);
        for (int r = 0; r < ROWS; ++r)
            if (board[r][c] == disc) {
                board[r][c] = EMPTY;
                break;
            }
        if (wins) return c;
    }
    return -1;
}

int choose_ai_move(char board[ROWS][COLS], enum Difficulty difficulty) {
    int moves[COLS];
    int count = get_valid_moves(board, moves);
    if (count == 0) return -1;

    if (difficulty == EASY)
        return moves[rand() % count];

    /* Medium and hard always take an immediate win or block a human win. */
    int win = immediate_winning_move(board, PLAYER2);
    if (win != -1) return win;
    int block = immediate_winning_move(board, PLAYER1);
    if (block != -1) return block;

    int depth = (difficulty == MEDIUM) ? MEDIUM_DEPTH : HARD_DEPTH;
    int best_col = moves[0];
    int best_score = INT_MIN;

    for (int i = 0; i < count; ++i) {
        int col = moves[i];
        drop_disc(board, col, PLAYER2);
        int score = minimax(board, depth - 1, INT_MIN, INT_MAX, 0);
        for (int r = 0; r < ROWS; ++r)
            if (board[r][col] == PLAYER2) {
                board[r][col] = EMPTY;
                break;
            }

        if (score > best_score) {
            best_score = score;
            best_col = col;
        }
    }
    return best_col;
}
