#ifndef TETRIC_H
#define TETRIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define WINDOW_WIDTH BOARD_WIDTH * BLOCK_SIZE * 3
#define WINDOW_HEIGHT (BOARD_HEIGHT + 10) * BLOCK_SIZE
#define BLOCK_SIZE 30
#define TETROMINO_SHAPE_BOX_SIZE 4
#define COLOR_COMPONENTS 3

typedef struct
{
    int occupied;
    int color[COLOR_COMPONENTS];
} Cell;

typedef struct {
    int x, y;
    int shape[TETROMINO_SHAPE_BOX_SIZE][TETROMINO_SHAPE_BOX_SIZE];
    int color[COLOR_COMPONENTS];
} Tetromino;

typedef struct {
    int x, y;
    Tetromino* currentTetromino;
    Cell ** gameBoard;
} Board;

typedef struct {
    SDL_Rect rect;
    const char* text;
    SDL_Color baseColor;
    SDL_Color hoverColor;
    SDL_Color currentColor;
    bool hovered;
} MenuButton;

Board * playerBoard;
Board * computerBoard;

SDL_Window* pWindow = NULL;
SDL_Renderer* renderer = NULL;

void init();

void draw(Board * board);

void drawBlock(int x, int y, int color[]);

bool canMove(Board * board, int dx, int dy);

Tetromino *getRandomTetromino();

Tetromino *rotateTetrominoLeft(Board * board);
int isPositionValid(Board * board, Tetromino *tetromino);

int checkLegalRotation(Board * board, Tetromino *tetromino);
void moveTetrominoOutOfBlock(Board * board, Tetromino *tetromino);
void moveTetrominoInbound(Tetromino *tetromino);
void moveInboundLeft(Tetromino *tetromino);
void moveInboundRight(Tetromino *tetromino);
void moveInboundUp(Tetromino *tetromino);

void placeTetromino(Board * board);

void moveTetromino(Board * board, int dx, int dy);

char checkLineFull(Board * board, int row);

void clearLine(Board * board, int row);

char valueinarray(int val, int *arr, size_t n);

void setCellColor(Board * board, int x, int y, int r, int g, int b);

Board * createBoard(int x, int y);

void freeBoard(Board * board);

#endif // TETRIC_H