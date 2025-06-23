#ifndef GAME_H
#define GAME_H

#include "tetris.h"
#include "utils.h"
#include "ui.h"

void singleGame();
void botGame();

// Fonctions liées aux Tetrominos
Tetromino *rotateTetrominoLeft(Board * board);

int tryMoveTetromino(Board *board, Tetromino *tetromino, int dx, int dy);

void placeTetromino(Board * board);

void moveTetromino(Board * board, int dx, int dy);
int isValidPosition(Board *board, Tetromino *tetromino);

int evaluateGameBoard(Cell** gameBoard);
int getBestMove(Board *board);

Cell** copyGameBoard(Cell** original);

void freeGameBoard(Cell** board);

// Fonctions liées au plateau
Board * createBoard(int x, int y);
void freeBoard(Board * board);
void draw(Board * board);
char checkLineFull(Board * board, int row);
void clearLine(Board * board, int row);

char hasLost(Board* board);

void setCellColor(Board * board, int x, int y, int r, int g, int b);

bool canMove(Board * board, int dx, int dy);

#endif // GAME_H