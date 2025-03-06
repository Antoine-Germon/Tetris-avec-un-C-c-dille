#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 30
#define TETROMINO_SHAPE_BOX_SIZE 4

int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};

SDL_Window* pWindow = NULL;
SDL_Renderer* renderer = NULL;

typedef struct {
    int x, y;
    int shape[TETROMINO_SHAPE_BOX_SIZE][TETROMINO_SHAPE_BOX_SIZE];
} Tetromino;

void init();

void draw();

bool canMove(int dx, int dy);

Tetromino* getRandomTetromino();

Tetromino* rotateTetrominoLeft();

void placeTetromino();

void moveTetromino(int dx, int dy);