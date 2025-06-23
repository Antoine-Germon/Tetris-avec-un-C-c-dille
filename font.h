#ifndef FONT_H
#define FONT_H

#include "tetris.h"

SDL_Texture* loadFontTexture(const char* path);
SDL_Rect getCharSrcRect(unsigned char c);
void drawChar(unsigned char c, int x, int y, SDL_Color color);
void drawText(const char* text, int x, int y, SDL_Color color);
void drawTextSpaced(const char* text, int x, int y, SDL_Color color, int charWidth, int charHeight, int spacing);

#endif // FONT_H