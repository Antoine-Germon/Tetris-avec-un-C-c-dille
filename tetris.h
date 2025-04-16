#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 30
#define TETROMINO_SHAPE_BOX_SIZE 4

typedef struct
{
    int occupied;
    int color[3];
} Cell;

Cell **board;

SDL_Window *pWindow = NULL;
SDL_Renderer *renderer = NULL;

typedef struct
{
    int x, y;
    int shape[TETROMINO_SHAPE_BOX_SIZE][TETROMINO_SHAPE_BOX_SIZE];
    int color[3];
} Tetromino;

void init();

void draw();

void drawBlock(int x, int y, int color[]);

bool canMove(int dx, int dy);

Tetromino *getRandomTetromino();

Tetromino *rotateTetrominoLeft();
void moveTetrominoInbound();
void moveInboundLeft();
void moveInboundRight();
void moveInboundUp();

void placeTetromino();

void moveTetromino(int dx, int dy);

char checkLineFull(int row);

void clearLine(int row);

char valueinarray(int val, int *arr, size_t n);

void setCellColor(int x, int y, int r, int g, int b);