#ifndef GAME_H
#define GAME_H

#include "tetris.h"
#include "utils.h"
#include "ui.h"

void singleGame();
void botGame();

// Fonctions liées aux Tetrominos
Tetromino *getRandomTetromino();

Tetromino *rotateTetrominoLeft(Board * board);

int isPositionValid(Board * board, Tetromino *tetromino);

int tryMoveTetromino(Board *board, Tetromino *tetromino, int dx, int dy);

void placeTetromino(Board * board);

void moveTetromino(Board * board, int dx, int dy);
int isValidPosition(Board *board, Tetromino *tetromino);
// Fonctions liées au plateaux
Board * createBoard(int x, int y);
void freeBoard(Board * board);
void draw(Board * board);
char checkLineFull(Board * board, int row);
void clearLine(Board * board, int row);

void setCellColor(Board * board, int x, int y, int r, int g, int b);

bool canMove(Board * board, int dx, int dy);

#endif // GAME_H