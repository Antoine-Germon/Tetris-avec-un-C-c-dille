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
        board->tetrominoBag[i] = i;  
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
void setCellColor(Board * board, int x, int y, int r, int g, int b)
{
    board->gameBoard[y][x].color[0] = r;
    board->gameBoard[y][x].color[1] = g;
    board->gameBoard[y][x].color[2] = b;
}
Board * createBoard(int x, int y)
{
    Board * board = malloc(sizeof(Board));
    board->score = 0;
    board->level = 1;
    board->linesCleared = 0;
    board->x = x;
    board->y = y;
    
    Cell ** gameBoard = malloc(BOARD_HEIGHT * sizeof(Cell *));
    if (!gameBoard) {
        printf("Failed to allocate memory for row pointers\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        gameBoard[i] = malloc(sizeof(Cell) * BOARD_WIDTH); 
        if (!gameBoard[i]) {
            printf("Failed to allocate memory for row\n");

            while (--i >= 0) free(gameBoard[i]);
            free(gameBoard);
            exit(EXIT_FAILURE);
        }
    }

    board->gameBoard = gameBoard;

    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            board->gameBoard[y][x].occupied = 0;
            setCellColor(board, x, y, 0, 0, 0);
        }
    }

    return board;
}

void freeBoard(Board * board)
{
    for (int i = 0; i < BOARD_HEIGHT; i++)
        free(board->gameBoard[i]);

    free(board->gameBoard);

    free(board);
}