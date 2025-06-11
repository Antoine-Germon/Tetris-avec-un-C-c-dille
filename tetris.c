#include "tetris.h"

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

#define CHAR_WIDTH 16
#define CHAR_HEIGHT 32

SDL_Texture* fontTexture = NULL;

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

// Affiche un seul caractère à l'écran
void drawChar(unsigned char c, int x, int y, SDL_Color color) {
    if (c < 32 || c > 127) return; // ou tu peux afficher un caractère "?"
    int index = c - 32; // on commence à l'index 0
    SDL_Rect srcRect = {
        (index % 16) * CHAR_WIDTH,
        (index / 16) * CHAR_HEIGHT,
        CHAR_WIDTH,
        CHAR_HEIGHT
    };
    SDL_Rect dstRect = { x, y, CHAR_WIDTH, CHAR_HEIGHT };
    SDL_RenderCopy(renderer, fontTexture, &srcRect, &dstRect);
}

// Affiche une chaîne de caractères
void drawText(const char* text, int x, int y, SDL_Color color) {
    for (int i = 0; text[i] != '\0'; i++) {
        drawChar(text[i], x + i * CHAR_WIDTH, y, color);
    }
}

void updateButtonHover(MenuButton* button, int mouseX, int mouseY) {
    bool inside = SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button->rect);
    button->hovered = inside;
    button->currentColor = inside ? button->hoverColor : button->baseColor;
}

void drawButton(MenuButton* button) {
    SDL_SetRenderDrawColor(renderer,
        button->currentColor.r,
        button->currentColor.g,
        button->currentColor.b,
        255);
    SDL_RenderFillRect(renderer, &button->rect);

    int len = 0;
    if (button->text != NULL)
        len = strlen(button->text);

    int textX = button->rect.x + (button->rect.w - len * CHAR_WIDTH) / 2;
    int textY = button->rect.y + (button->rect.h - CHAR_HEIGHT) / 2;

    drawText(button->text, textX, textY, (SDL_Color){255, 255, 255});
}

// Main menu loop
void showMainMenu() {
    MenuButton soloButton = {
        .rect = {WINDOW_WIDTH / 2 - 100, 200, 200, 50},
        .text = "Jouer en solo",
        .baseColor = {70, 130, 180},
        .hoverColor = {100, 149, 237},
        .currentColor = {70, 130, 180},
        .hovered = false
    };

    MenuButton vsBotButton = {
        .rect = {WINDOW_WIDTH / 2 - 100, 300, 200, 50},
        .text = "Jouer contre IA",
        .baseColor = {34, 139, 34},
        .hoverColor = {60, 179, 113},
        .currentColor = {34, 139, 34},
        .hovered = false
    };

    bool running = true;
    SDL_Event event;
    int mouseX, mouseY;

    while (running) {
        SDL_GetMouseState(&mouseX, &mouseY);
        updateButtonHover(&soloButton, mouseX, mouseY);
        updateButtonHover(&vsBotButton, mouseX, mouseY);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return;
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (soloButton.hovered) {
                    printf("Solo sélectionné\n");
                    return;
                } else if (vsBotButton.hovered) {
                    printf("VS Bot sélectionné\n");
                    return;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        drawButton(&soloButton);
        drawButton(&vsBotButton);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    init();

    /* fontTexture = loadFontTexture("./ascii.bmp");

    if (!fontTexture) {
        printf("Erreur : fontTexture NULL\n");
        return 1;
    }
    
    showMainMenu(); */
    
    Board * playerBoard = createBoard(0, 0);
    Board * botBoard = createBoard(20, 0);
    
    playerBoard->currentTetromino = getRandomTetromino();
    botBoard->currentTetromino = getRandomTetromino();
    
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
                    moveTetromino(playerBoard, -1, 0);
                    break;
                case SDLK_RIGHT:
                    moveTetromino(playerBoard, 1, 0);
                    break;
                case SDLK_DOWN:
                    moveTetromino(playerBoard, 0, 1);
                    break;
                case SDLK_r:
                {
                    Tetromino *rotated = rotateTetrominoLeft(playerBoard);
                    if (rotated != playerBoard->currentTetromino)
                    {
                        free(playerBoard->currentTetromino);
                        playerBoard->currentTetromino = rotated;
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
            moveTetromino(playerBoard, 0, 1);
            moveTetromino(botBoard, 0, 1);
            lastFallTime = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);    

        draw(playerBoard);
        draw(botBoard);

        SDL_RenderPresent(renderer);

        SDL_Delay(50);
    }

    SDL_DestroyTexture(fontTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    
    freeBoard(playerBoard);
    freeBoard(botBoard);

    return 0;
}

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
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
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

void draw(Board * board)
{
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int offsetX = board->x;
    int offsetY = board->y;
    
    // Draw board
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (board->gameBoard[y][x].occupied)
            {
                drawBlock(offsetX + x, offsetY + y, board->gameBoard[y][x].color);
            }
        }
    }

    Tetromino * currentTetromino = board->currentTetromino;

    // Draw the current Tetromino
    SDL_SetRenderDrawColor(renderer, currentTetromino->color[0], currentTetromino->color[1], currentTetromino->color[2], 255);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (currentTetromino->shape[i][j])
            {
                drawBlock(currentTetromino->x + j + offsetX, currentTetromino->y + i + offsetY, currentTetromino->color);
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
}

// Move the tetromino down
bool canMove(Board * board, int dx, int dy)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (board->currentTetromino->shape[i][j])
            {
                int newX = board->currentTetromino->x + j + dx;
                int newY = board->currentTetromino->y + i + dy;

                if (newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT || board->gameBoard[newY][newX].occupied)
                    return false;
            }
        }
    }
    return true;
}

Tetromino *getRandomTetromino()
{
    int index = rand() % (sizeof(tetrominos) / sizeof(Tetromino));
    Tetromino *copy = malloc(sizeof(Tetromino));
    if (!copy)
    {
        printf("Failed to allocate memory for Tetromino copy\n");
        exit(EXIT_FAILURE);
    }

    *copy = tetrominos[index]; // Copie complète (shallow copy)

    return copy;
}

Tetromino* rotateTetrominoLeft(Board * board) {
    Tetromino* tempTetromino = malloc(sizeof(Tetromino));

    tempTetromino->x = board->currentTetromino->x;
    tempTetromino->y = board->currentTetromino->y;
    tempTetromino->color[0] = board->currentTetromino->color[0];
    tempTetromino->color[1] = board->currentTetromino->color[1];
    tempTetromino->color[2] = board->currentTetromino->color[2];

    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++) {
            tempTetromino->shape[i][j] = board->currentTetromino->shape[j][TETROMINO_SHAPE_BOX_SIZE - i - 1];
        }
    }
    moveTetrominoOutOfBlock(board, tempTetromino);
    moveTetrominoInbound(tempTetromino);
    int canRotate = checkLegalRotation(board, tempTetromino);
    if (canRotate == 1)
    {
        return tempTetromino;
    }
    else
    {
        return board->currentTetromino;
    }
}
int checkLegalRotation(Board * board, Tetromino *tetromino)
{
    int posX = tetromino->x;
    int posY = tetromino->y;
    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++)
    {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++)
        {
            if (posY + j < BOARD_HEIGHT && posX + i < BOARD_WIDTH)
            {

                if (board->gameBoard[posY + j][posX + i].occupied == 1 && tetromino->shape[j][i] == 1)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}
void moveTetrominoOutOfBlock(Board * board, Tetromino *tetromino)
{
    int posX = tetromino->x;
    int posY = tetromino->y;
    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++)
    {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++)
        {
            if (posY + j < BOARD_HEIGHT && posX + i < BOARD_WIDTH)
            {
                if (board->gameBoard[posY + j][posX + i].occupied == 1 && tetromino->shape[j][i] == 1)
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

void setCellColor(Board * board, int x, int y, int r, int g, int b)
{
    board->gameBoard[x][y].color[0] = r;
    board->gameBoard[x][y].color[1] = g;
    board->gameBoard[x][y].color[2] = b;
}

Board * createBoard(int x, int y)
{
    Board * board = malloc(sizeof(Board));

    board->x = x;
    board->y = y;
    
    Cell ** gameBoard = malloc(BOARD_HEIGHT * sizeof(Cell *));
    if (!gameBoard) {
        printf("Failed to allocate memory for row pointers\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        gameBoard[i] = malloc(sizeof(Cell) * BOARD_WIDTH); // calloc initializes to 0
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
            //printf("x: %d, y: %d, board[x][y]: %d\n", x, y, board->gameBoard[y][x]);
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

void placeTetromino(Board * board)
{
    int checkedLines[BOARD_HEIGHT] = {0};
    int nbOfCheckedLines = 0;

    Tetromino * currentTetromino = board->currentTetromino;

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

            board->gameBoard[currentTetromino->y + i][currentTetromino->x + j].occupied = 1;
            setCellColor(
                board,
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
        char lineFull = checkLineFull(board, row);
        if (lineFull) clearLine(board, row);
    }

    board->currentTetromino = getRandomTetromino();
    board->currentTetromino->x = 3;
    board->currentTetromino->y = 0;
}

void moveTetromino(Board * board, int dx, int dy)
{
    if (canMove(board, dx, dy))
    {
        board->currentTetromino->x += dx;
        board->currentTetromino->y += dy;
    }
    else if (dy > 0) // If moving down is blocked, place the piece
    {
        printf("PLAAAAACE\n");
        placeTetromino(board);
    }
}

char checkLineFull(Board * board, int row)
{
    for (int i = 0; i < BOARD_WIDTH; i++) {
        if (!board->gameBoard[row][i].occupied) {
            return 0;
        }
    }
    return 1;
}

void clearLine(Board * board, int row)
{
    for (int i = 0; i < BOARD_WIDTH; i++) {
        board->gameBoard[row][i].occupied = 0;
    }

    for (int i = row; i > 0; i--) {
        Cell * tmp = board->gameBoard[i - 1];
        board->gameBoard[i] = board->gameBoard[i - 1];
        board->gameBoard[i - 1] = tmp;
    }
}
