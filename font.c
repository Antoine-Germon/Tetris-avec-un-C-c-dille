#include "font.h"

// Charge l'image ascii.bmp
SDL_Texture* loadFontTexture(const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
        printf("Erreur chargement BMP: %s\n", SDL_GetError());
        return NULL;
    }
    // Remplace le noir par transparent
    Uint32 colorkey = SDL_MapRGB(surface->format, 0, 0, 0);
    SDL_SetColorKey(surface, SDL_TRUE, colorkey);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Rect getCharSrcRect(unsigned char c) {
    if (c < 0x20 || c > 0x9F) {
        SDL_Rect empty = {0, 0, 0, 0};
        return empty;
    }

    int index = c - 0x20;
    int row = index / SPRITESHEET_COLS;
    int col = index % SPRITESHEET_COLS;

    SDL_Rect rect = {
        .x = col * CHAR_WIDTH,
        .y = row * CHAR_HEIGHT,
        .w = CHAR_WIDTH,
        .h = CHAR_HEIGHT
    };

    return rect;
}

// Affiche un seul caractère à l'écran
void drawChar(unsigned char c, int x, int y, SDL_Color color) {
    SDL_Rect srcRect = getCharSrcRect(c);
    SDL_Rect dstRect = { x, y, CHAR_WIDTH, CHAR_HEIGHT };
    SDL_RenderCopy(renderer, fontTexture, &srcRect, &dstRect);
}

// Affiche une chaîne de caractères
void drawText(const char* text, int x, int y, SDL_Color color) {
    for (int i = 0; text[i] != '\0'; i++) {
        drawChar(text[i], x + i * CHAR_WIDTH, y, color);
    }
}

void drawTextSpaced(const char* text, int x, int y, SDL_Color color, int charWidth, int charHeight, int spacing) {
    for (int i = 0; text[i] != '\0'; i++) {
        SDL_Rect srcRect = getCharSrcRect(text[i]);
        SDL_Rect dstRect = { x + i * (charWidth + spacing), y, charWidth, charHeight };
        SDL_RenderCopy(renderer, fontTexture, &srcRect, &dstRect);
    }
}