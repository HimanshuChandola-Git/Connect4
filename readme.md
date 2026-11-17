# Connect 4 Game in C

A command-line implementation of the classic Connect 4 game, developed using C.

## Features

- Player vs Player mode
- Player vs Computer mode
- Three AI difficulty levels:
  - Easy
  - Medium
  - Hard
- Minimax algorithm with Alpha-Beta Pruning
- Board evaluation using heuristic scoring
- Immediate winning move detection and blocking

## Technologies Used

- C Programming
- GCC Compiler
- Minimax Algorithm
- Alpha-Beta Pruning

## How to Compile

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic main.c board.c ai.c -o connect4
```

## How to Run

Windows:

```bash
connect4.exe
```

Linux/macOS:

```bash
./connect4
```

## Author

Himanshu