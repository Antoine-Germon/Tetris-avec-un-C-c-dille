#ifndef GAME_H
#define GAME_H

#include "tetris.h"
#include "utils.h"
#include "ui.h"
#include "bot.h"
void singleGame();
void botGame();

Tetromino *rotateTetrominoLeft(Board * board);

int tryMoveTetromino(Board *board, Tetromino *tetromino, int dx, int dy);

int placeTetromino(Board * board);

void moveTetromino(Board * board, int dx, int dy, Board* opponent);
int isValidPosition(Board *board, Tetromino *tetromino);

void addGarbageLine(Board* board, int numberOfRows);

void freeGameBoard(Cell** board);

char checkLineFull(Board * board, int row);
void clearLine(Board * board, int row);

char hasLost(Board* board);

void setCellColor(Board * board, int x, int y, int r, int g, int b);

bool canMove(Board * board, int dx, int dy);

#endif // GAME_H