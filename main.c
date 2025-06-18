#include "game.h"
#include "font.h"

void init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pWindow = SDL_CreateWindow(
        "Tetris",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        MULTIPLAYER_WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    init();

    fontTexture = loadFontTexture("./ascii.bmp");

    if (!fontTexture) {
        printf("Erreur : fontTexture NULL\n");
        return 1;
    }
    
    showMainMenu();

    return 0;
}