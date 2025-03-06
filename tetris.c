#include "tetris.h"

Tetromino* currentTetromino;

Tetromino tetrominos[] = {
    // Square
    { .x = 3, .y = 0, .shape = {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    }},
    // Line
    { .x = 3, .y = 0, .shape = {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    }},
    // T-shape
    { .x = 3, .y = 0, .shape = {
        {0, 0, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
    }},
    // L-shape
    { .x = 3, .y = 0, .shape = {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    }},
    // Reverse L-shape
    { .x = 3, .y = 0, .shape = {
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    }},
    // Z-shape
    { .x = 3, .y = 0, .shape = {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 0}
    }},
    // Reverse Z-shape
    { .x = 3, .y = 0, .shape = {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    }}
};

int main(int argc, char** argv)
{
    init();

    currentTetromino = getRandomTetromino();

    bool quit = false;
    Uint32 lastFallTime = SDL_GetTicks();

    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        moveTetromino(-1, 0);
                        break;
                    case SDLK_RIGHT:
                        moveTetromino(1, 0);
                        break;
                    case SDLK_DOWN:
                        moveTetromino(0, 1);
                        break;
                    case SDLK_r:
                        currentTetromino = rotateTetrominoLeft();
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            }
        }

        // Move tetromino down every 500ms
        if (SDL_GetTicks() - lastFallTime > 500)
        {
            moveTetromino(0, 1);
            lastFallTime = SDL_GetTicks();
        }

        draw();
        SDL_Delay(50);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}

void init()
{
    SDL_Init(SDL_INIT_VIDEO);
    pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
}

void draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // Draw board
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (board[y][x])
            {
                SDL_Rect block = { x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
                SDL_RenderFillRect(renderer, &block);
            }
        }
    }

    // Draw the current Tetromino
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (currentTetromino->shape[i][j])
            {
                SDL_Rect block = {
                    (currentTetromino->x + j) * BLOCK_SIZE,
                    (currentTetromino->y + i) * BLOCK_SIZE,
                    BLOCK_SIZE,
                    BLOCK_SIZE
                };
                SDL_RenderFillRect(renderer, &block);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

// Move the tetromino down
bool canMove(int dx, int dy)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (currentTetromino->shape[i][j])
            {
                int newX = currentTetromino->x + j + dx;
                int newY = currentTetromino->y + i + dy;

                if (newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT || board[newY][newX])
                    return false;
            }
        }
    }
    return true;
}

Tetromino* getRandomTetromino() {
    int index = rand() % (sizeof(tetrominos) / sizeof(Tetromino));
    return &tetrominos[index];
}

Tetromino* rotateTetrominoLeft() {
    Tetromino* tempTetromino = malloc(sizeof(Tetromino));

    tempTetromino->x = currentTetromino->x;
    tempTetromino->y = currentTetromino->y;

    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++) {
            tempTetromino->shape[i][j] = currentTetromino->shape[j][TETROMINO_SHAPE_BOX_SIZE - i - 1];
        }
    }

    return tempTetromino;
}

void placeTetromino()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (currentTetromino->shape[i][j])
            {
                board[currentTetromino->y + i][currentTetromino->x + j] = 1;
            }
        }
    }
    currentTetromino = getRandomTetromino();
}

void moveTetromino(int dx, int dy)
{
    if (canMove(dx, dy))
    {
        currentTetromino->x += dx;
        currentTetromino->y += dy;
    }
    else if (dy > 0) // If moving down is blocked, place the piece
    {
        placeTetromino();
    }
}