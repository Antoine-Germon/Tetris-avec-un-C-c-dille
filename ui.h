#ifndef UI_H
#define UI_H

#include "tetris.h"
#include "font.h"

void showMainMenu();
void updateButtonHover(MenuButton* button, int mouseX, int mouseY);
void drawButton(MenuButton* button);
void drawBlock(int x, int y, int color[]);

#endif // UI_H