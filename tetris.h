#ifndef TETRIS_H
#define TETRIS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define MULTIPLAYER_WINDOW_WIDTH BOARD_WIDTH * BLOCK_SIZE * 3
#define SINGLEPLAYER_WINDOW_WIDTH BOARD_WIDTH * BLOCK_SIZE * 2
#define WINDOW_HEIGHT (BOARD_HEIGHT + 5) * BLOCK_SIZE
#define BLOCK_SIZE 30
#define TETROMINO_SHAPE_BOX_SIZE 4
#define COLOR_COMPONENTS 3

#define CHAR_WIDTH 32
#define CHAR_HEIGHT 32

#define SPRITESHEET_COLS 16
#define SPRITESHEET_ROWS 8
#define BAG_SIZE 7


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
    Tetromino* nextTetromino;
    Cell ** gameBoard;
    int tetrominoBag[BAG_SIZE];
    int bagIndex;
} Board;

typedef struct {
    SDL_Rect rect;
    const char* text;
    SDL_Color baseColor;
    SDL_Color hoverColor;
    SDL_Color currentColor;
    bool hovered;
} MenuButton;

extern Tetromino tetrominos[];
extern const int tetrominoCount;
extern SDL_Texture* fontTexture;
extern Board* playerBoard;
extern Board* computerBoard;
extern SDL_Window* pWindow;
extern SDL_Renderer* renderer;

void refillBag(Board* board);
Tetromino* getNextTetromino(Board* board);
#endif // TETRIS_H