#include "tetris.h"
Tetromino tetrominos[] = {
    {.x = 3, .y = 0, .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, .color = {255, 255, 0}},
    {.x = 3, .y = 0, .shape = {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}, .color = {0, 255, 255}},
    {.x = 3, .y = 0, .shape = {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}, .color = {128, 0, 128}},
    {.x = 3, .y = 0, .shape = {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, .color = {255, 165, 0}},
    {.x = 3, .y = 0, .shape = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, .color = {0, 0, 255}},
    {.x = 3, .y = 0, .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}}, .color = {255, 0, 0}},
    {.x = 3, .y = 0, .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}, .color = {0, 255, 0}}
};

SDL_Texture* fontTexture = NULL;
Board* playerBoard = NULL;
Board* computerBoard = NULL;
SDL_Window* pWindow = NULL;
SDL_Renderer* renderer = NULL;

const int tetrominoCount = sizeof(tetrominos) / sizeof(tetrominos[0]);

void refillBag(Board* board) {
    for (int i = 0; i < BAG_SIZE; i++) {
        board->tetrominoBag[i] = i;  // Indices 0 à 6 correspondant aux 7 tetrominos
    }

    // Shuffle avec Fisher-Yates
    for (int i = BAG_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = board->tetrominoBag[i];
        board->tetrominoBag[i] = board->tetrominoBag[j];
        board->tetrominoBag[j] = temp;
    }

    board->bagIndex = 0;
}
Tetromino* getNextTetromino(Board* board) {
    if (board->bagIndex >= BAG_SIZE) {
        refillBag(board);
    }

    int index = board->tetrominoBag[board->bagIndex++];
    Tetromino *copy = malloc(sizeof(Tetromino));
    if (!copy) {
        printf("Failed to allocate memory for Tetromino copy\n");
        exit(EXIT_FAILURE);
    }

    *copy = tetrominos[index];
    return copy;
}