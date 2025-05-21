#include "tetris.h"

Tetromino *currentTetromino;

Tetromino tetrominos[] = {
    // Square
    {.x = 3, .y = 0, .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, .color = {255, 255, 0}}, // Yellow

    // Line
    {.x = 3, .y = 0, .shape = {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}, .color = {0, 255, 255}}, // Cyan

    // T-shape
    {.x = 3, .y = 0, .shape = {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}, .color = {128, 0, 128}}, // Purple

    // L-shape
    {.x = 3, .y = 0, .shape = {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, .color = {255, 165, 0}}, // Orange

    // Reverse L-shape
    {.x = 3, .y = 0, .shape = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, .color = {0, 0, 255}}, // Blue

    // Z-shape
    {.x = 3, .y = 0, .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}}, .color = {255, 0, 0}}, // Red

    // Reverse Z-shape
    {.x = 3, .y = 0, .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}, .color = {0, 255, 0}} // Green
};

int main(int argc, char **argv)
{
    srand(time(NULL));

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
                {
                    Tetromino *rotated = rotateTetrominoLeft();
                    if (rotated != currentTetromino)
                    {
                        free(currentTetromino);
                        currentTetromino = rotated;
                    }
                    else
                    {
                        free(rotated); // Important si tu fais un malloc() inutilement dans tous les cas
                    }
                    break;
                }
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

    for (int i = 0; i < BOARD_HEIGHT; i++)
        free(board[i]);

    free(board);

    return 0;
}

void init()
{
    board = malloc(BOARD_HEIGHT * sizeof(Cell *));
    if (!board)
    {
        printf("Failed to allocate memory for row pointers\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        board[i] = calloc(BOARD_WIDTH, sizeof(Cell)); // calloc initializes to 0
        if (!board[i])
        {
            printf("Failed to allocate memory for row\n");

            while (--i >= 0)
                free(board[i]);
            free(board);
            exit(EXIT_FAILURE);
        }
    }

    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            board[y][x].occupied = 0;
            setCellColor(x, y, 0, 0, 0);
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
}

void drawBlock(int x, int y, int color[])
{
    SDL_Rect outsideBlock = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};

    int r = color[0];
    int g = color[1];
    int b = color[2];

    SDL_SetRenderDrawColor(renderer, r - r * 0.25, g - g * 0.25, b - b * 0.25, 255);

    SDL_RenderFillRect(renderer, &outsideBlock);

    double offset = 0.6;

    SDL_Rect insideBlock = {x * BLOCK_SIZE + (BLOCK_SIZE * (1 - offset) / 2), y * BLOCK_SIZE + (BLOCK_SIZE * (1 - offset) / 2), BLOCK_SIZE * offset, BLOCK_SIZE * offset};

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    SDL_RenderFillRect(renderer, &insideBlock);
}

void draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw board
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (board[y][x].occupied)
            {
                drawBlock(x, y, board[y][x].color);
            }
        }
    }

    // Draw the current Tetromino
    SDL_SetRenderDrawColor(renderer, currentTetromino->color[0], currentTetromino->color[1], currentTetromino->color[2], 255);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (currentTetromino->shape[i][j])
            {
                drawBlock(currentTetromino->x + j, currentTetromino->y + i, currentTetromino->color);
                /* SDL_Rect block = {
                    (currentTetromino->x + j) * BLOCK_SIZE,
                    (currentTetromino->y + i) * BLOCK_SIZE,
                    BLOCK_SIZE,
                    BLOCK_SIZE
                };
                SDL_RenderFillRect(renderer, &block); */
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

                if (newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT || board[newY][newX].occupied)
                    return false;
            }
        }
    }
    return true;
}

Tetromino *getRandomTetromino()
{
    int index = rand() % (sizeof(tetrominos) / sizeof(Tetromino));
    return &tetrominos[index];
}

Tetromino *rotateTetrominoLeft()
{
    Tetromino *tempTetromino = malloc(sizeof(Tetromino));

    tempTetromino->x = currentTetromino->x;
    tempTetromino->y = currentTetromino->y;
    tempTetromino->color[0] = currentTetromino->color[0];
    tempTetromino->color[1] = currentTetromino->color[1];
    tempTetromino->color[2] = currentTetromino->color[2];

    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++)
    {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++)
        {
            tempTetromino->shape[i][j] = currentTetromino->shape[j][TETROMINO_SHAPE_BOX_SIZE - i - 1];
        }
    }
    moveTetrominoOutOfBlock(tempTetromino);
    moveTetrominoInbound(tempTetromino);
    int canRotate = checkLegalRotation(tempTetromino);
    if (canRotate == 1)
    {
        return tempTetromino;
    }
    else
    {
        return currentTetromino;
    }
}
int checkLegalRotation(Tetromino *tetromino)
{
    int posX = tetromino->x;
    int posY = tetromino->y;
    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++)
    {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++)
        {
            if (posY + j < BOARD_HEIGHT && posX + i < BOARD_WIDTH)
            {

                if (board[posY + j][posX + i].occupied == 1 && tetromino->shape[j][i] == 1)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}
void moveTetrominoOutOfBlock(Tetromino *tetromino)
{
    int posX = tetromino->x;
    int posY = tetromino->y;
    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++)
    {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++)
        {
            if (posY + j < BOARD_HEIGHT && posX + i < BOARD_WIDTH)
            {
                if (board[posY + j][posX + i].occupied == 1 && tetromino->shape[j][i] == 1)
                {
                    if (i < 2)
                    {
                        tetromino->x = i;
                        return;
                    }
                    else
                    {
                        tetromino->x -= i;
                        return;
                    }
                }
            }
        }
    }
}

void moveTetrominoInbound(Tetromino *tetromino)
{
    if (tetromino->x < 0)
    {
        moveInboundLeft(tetromino);
    }
    else if (tetromino->x + TETROMINO_SHAPE_BOX_SIZE - 1 >= BOARD_WIDTH)
    {
        moveInboundRight(tetromino);
    }
    if (tetromino->y + TETROMINO_SHAPE_BOX_SIZE - 1 >= BOARD_HEIGHT)
    {
        moveInboundUp(tetromino);
    }
}

void moveInboundLeft(Tetromino *tetromino)
{
    for (int i = 0; i + tetromino->x < 0; i++)
    {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++)
        {
            if (tetromino->shape[j][i] == 1)
            {
                tetromino->x = i;
                return;
            }
        }
    }
}

void moveInboundRight(Tetromino *tetromino)
{
    for (int i = TETROMINO_SHAPE_BOX_SIZE - 1; i + tetromino->x >= BOARD_WIDTH; i--)
    {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++)
        {
            if (tetromino->shape[j][i] == 1)
            {
                tetromino->x = BOARD_WIDTH - i - 1;
                return;
            }
        }
    }
}
void moveInboundUp(Tetromino *tetromino)
{
    for (int j = TETROMINO_SHAPE_BOX_SIZE - 1; j + tetromino->y >= BOARD_HEIGHT; j--)
    {
        for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++)
        {
            if (tetromino->shape[j][i] == 1)
            {
                tetromino->y = BOARD_HEIGHT - j - 1;
                return;
            }
        }
    }
}
char valueinarray(int val, int *arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}

void setCellColor(int x, int y, int r, int g, int b)
{
    board[x][y].color[0] = r;
    board[x][y].color[1] = g;
    board[x][y].color[2] = b;
}

void placeTetromino()
{
    int checkedLines[BOARD_HEIGHT] = {0};
    int nbOfCheckedLines = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (!currentTetromino->shape[i][j])
                continue;

            if (!valueinarray(currentTetromino->y + i, checkedLines, BOARD_HEIGHT))
            {
                checkedLines[nbOfCheckedLines] = currentTetromino->y + i;
                nbOfCheckedLines++;
            }

            board[currentTetromino->y + i][currentTetromino->x + j].occupied = 1;
            setCellColor(
                currentTetromino->y + i,
                currentTetromino->x + j,
                currentTetromino->color[0],
                currentTetromino->color[1],
                currentTetromino->color[2]);
        }
    }

    for (int i = 0; i < nbOfCheckedLines; i++)
    {
        int row = checkedLines[i];
        char lineFull = checkLineFull(row);
        if (lineFull)
            clearLine(row);
    }

    currentTetromino = getRandomTetromino();
    currentTetromino->x = 3;
    currentTetromino->y = 0;
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

char checkLineFull(int row)
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        if (!board[row][i].occupied)
        {
            return 0;
        }
    }
    return 1;
}

void clearLine(int row)
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        board[row][i].occupied = 0;
    }

    for (int i = row; i > 0; i--)
    {
        Cell *tmp = board[i - 1];
        board[i] = board[i - 1];
        board[i - 1] = tmp;
    }
}
