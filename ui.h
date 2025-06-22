#ifndef UI_H
#define UI_H

#include "tetris.h"
#include "font.h"
#define Y_OFFSET 20
#define X_OFFSET 15
void showMainMenu();
void updateButtonHover(MenuButton* button, int mouseX, int mouseY);
void drawButton(MenuButton* button);
void drawBlock(int x, int y, int color[]);
void drawGrid();
void drawBoardBorder();
void drawScore(int score);
void drawNextTetromino(Tetromino* next);
#endif // UI_H