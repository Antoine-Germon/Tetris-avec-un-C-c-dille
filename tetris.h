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

typedef struct {
    int x, y;
    int shape[TETROMINO_SHAPE_BOX_SIZE][TETROMINO_SHAPE_BOX_SIZE];
    int color[3];
} Tetromino;

typedef struct {
    int x, y;
    Tetromino* currentTetromino;
    Cell ** gameBoard;
} Board;

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