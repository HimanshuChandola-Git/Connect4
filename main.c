#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "board.h"
#include "ai.h"

static int read_integer(const char *prompt, int min, int max) {
    char line[128];
    char *end;
    long value;

    for (;;) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) return min - 1;

        value = strtol(line, &end, 10);
        while (isspace((unsigned char)*end)) ++end;

        if (end != line && *end == '\0' && value >= min && value <= max)
            return (int)value;

        printf("Please enter a number from %d to %d.\n", min, max);
    }
}

static void play_game(int vs_computer) {
    char board[ROWS][COLS];
    int current_player = 1;
    int difficulty = EASY;

    if (vs_computer) {
        puts("\nDifficulty: 1. Easy  2. Medium  3. Hard");
        difficulty = read_integer("Choose difficulty: ", 1, 3);
        if (difficulty < 1) return;
    }

    initialize_board(board);

    for (;;) {
        display_board(board);

        int col;
        char disc = (current_player == 1) ? PLAYER1 : PLAYER2;

        if (vs_computer && current_player == 2) {
            puts("Computer is thinking...");
            col = choose_ai_move(board, (enum Difficulty)difficulty);
            if (col < 0) break;
            printf("Computer chooses column %d.\n", col + 1);
        } else {
            char prompt[80];
            snprintf(prompt, sizeof(prompt),
                     "Player %d (%c), choose a column (1-7): ",
                     current_player, disc);
            col = read_integer(prompt, 1, COLS) - 1;
            if (col < 0) {
                puts("\nInput ended. Returning to menu.");
                return;
            }
            if (!drop_disc(board, col, disc)) {
                puts("That column is full. Choose another column.");
                continue;
            }
            /* The move has already been made. */
            if (has_won(board, disc)) {
                display_board(board);
                printf("Player %d (%c) wins!\n", current_player, disc);
                return;
            }
            if (is_board_full(board)) {
                display_board(board);
                puts("It's a draw!");
                return;
            }
            current_player = (current_player == 1) ? 2 : 1;
            continue;
        }

        if (!drop_disc(board, col, disc)) {
            puts("Computer selected an invalid move. Returning to menu.");
            return;
        }

        if (has_won(board, disc)) {
            display_board(board);
            if (vs_computer && current_player == 2)
                puts("Computer wins!");
            else
                printf("Player %d (%c) wins!\n", current_player, disc);
            return;
        }

        if (is_board_full(board)) {
            display_board(board);
            puts("It's a draw!");
            return;
        }

        current_player = (current_player == 1) ? 2 : 1;
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    puts("================================");
    puts("          CONNECT 4");
    puts("================================");

    for (;;) {
        puts("\n1. Player vs Player");
        puts("2. Player vs Computer");
        puts("3. Exit");

        int choice = read_integer("Select an option: ", 1, 3);
        if (choice == 1) {
            play_game(0);
        } else if (choice == 2) {
            play_game(1);
        } else {
            puts("Thanks for playing Connect 4!");
            break;
        }
    }

    return 0;
}
