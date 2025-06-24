#ifndef BOT_H
#define BOT_H
#include "tetris.h"
#include "utils.h"
#include "ui.h"
#include "game.h"

Cell** copyGameBoard(Cell** original);
int evaluateGameBoard(Cell** gameBoard);
int getBestMove(Board *board);
#endif
