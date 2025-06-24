#ifndef UI_H
#define UI_H

#include "tetris.h"
#include "font.h"
#define Y_OFFSET 20
#define X_OFFSET 15
typedef void (*RestartCallback)();
void showMainMenu();
void updateButtonHover(MenuButton* button, int mouseX, int mouseY);
void drawButton(MenuButton* button);
void drawBlock(int x, int y, int color[]);
void drawGrid(Board* board);
void drawBoardBorder(Board* board);
void drawScore(int x, int y, int score);
void drawNextTetromino(int x, int y, Tetromino* next);
void drawPlayerMenu(int x, int y, int score, Tetromino* next);
void showGameOverScreen(const char* text, RestartCallback restartFunction,int width);
#endif // UI_H