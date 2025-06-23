#include "game.h"

void singleGame() {
    playerBoard = createBoard(0, 1);
        
    refillBag(playerBoard);
    playerBoard->currentTetromino = getNextTetromino(playerBoard);
    playerBoard->nextTetromino = getNextTetromino(playerBoard);
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
                    lastFallTime = SDL_GetTicks();
                    break;
                case SDLK_r:
                {
                    Tetromino *rotated = rotateTetrominoLeft(playerBoard);
                    if (rotated != NULL) {
                        free(playerBoard->currentTetromino);
                        playerBoard->currentTetromino = rotated;
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

        if (hasLost(playerBoard)) {
            printf("You lost.............\n");
            break;
        }

        // Move tetromino down every 500ms
        if (SDL_GetTicks() - lastFallTime > 500)
        {
            moveTetromino(playerBoard, 0, 1);

            lastFallTime = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);    
        draw(playerBoard);
        /* drawScore(50);
        drawNextTetromino(playerBoard->currentTetromino); */

        drawPlayerMenu(X_OFFSET + BOARD_WIDTH * BLOCK_SIZE + 20, Y_OFFSET, 50, playerBoard->nextTetromino);
        SDL_RenderPresent(renderer);

        SDL_Delay(50);
    }

    SDL_DestroyTexture(fontTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    
    freeBoard(playerBoard);
}

void botGame() {
    playerBoard = createBoard(0, 1);
    computerBoard = createBoard(19, 1);
    refillBag(playerBoard);
    refillBag(computerBoard);

    playerBoard->currentTetromino = getNextTetromino(playerBoard);
    playerBoard->nextTetromino = getNextTetromino(playerBoard);
    computerBoard->currentTetromino = getNextTetromino(computerBoard);
    computerBoard->nextTetromino = getNextTetromino(computerBoard);
    
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
                    if (rotated != NULL) {
                        free(playerBoard->currentTetromino);
                        playerBoard->currentTetromino = rotated;
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

        if (hasLost(playerBoard)) {
            printf("Computer wins!!!!!!!!!!!!\n");
            break;
        }

        if (hasLost(computerBoard)) {
            printf("Player wins!!!!!!!!!!!!\n");
            break;
        }

        // Move tetromino down every 500ms
        if (SDL_GetTicks() - lastFallTime > 500)
        {
            moveTetromino(playerBoard, 0, 1);
            moveTetromino(computerBoard, 0, 1);
            
            lastFallTime = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);    

        draw(playerBoard);
        draw(computerBoard);

        drawPlayerMenu(X_OFFSET + BOARD_WIDTH * BLOCK_SIZE + 20, Y_OFFSET, 50, playerBoard->currentTetromino);
        SDL_RenderPresent(renderer);

        SDL_Delay(50);
    }

    SDL_DestroyTexture(fontTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    
    freeBoard(playerBoard);
    freeBoard(computerBoard);
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
    if (isValidPosition(board, tempTetromino)) {
        return tempTetromino;  // Rotation is valid, return rotated tetromino
    }

    // Rotation failed, try wall kicks (translation)
    const int kickTests[][2] = {
        {-1, 0}, {1, 0}, {0, 1},{0, -1},{-2, 0}, {2, 0},{0, -2}
    };

    for (int i = 0; i < 3; i++) {
        int dx = kickTests[i][0];
        int dy = kickTests[i][1];
        tempTetromino->x += dx;
        tempTetromino->y += dy;
        if (isValidPosition(board, tempTetromino)) {
            return tempTetromino;
        }
        tempTetromino->x -= dx; // Revenir si pas bon
        tempTetromino->y -= dy;
    }
    // If all moves fail, return the original tetromino
    free(tempTetromino);
    return NULL;
 
}

int tryMoveTetromino(Board *board, Tetromino *tetromino, int dx, int dy) {
    Tetromino tempTetromino = *tetromino;
    tempTetromino.x += dx;
    tempTetromino.y += dy;

    return isValidPosition(board, &tempTetromino);
}

int isValidPosition(Board *board, Tetromino *tetromino) {
    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++) {
            if (tetromino->shape[i][j]) {
                int x = tetromino->x + j;
                int y = tetromino->y + i;

                // Check bounds
                if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
                    printf("out of bounds\n");
                    return 0;
                }

                // Check collision with existing blocks
                if (board->gameBoard[y][x].occupied) {
                    printf("collision\n");
                    return 0;
                }
            }
        }
    }
    return 1;
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
                currentTetromino->x + j,
                currentTetromino->y + i,
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
    free(board->currentTetromino);
    board->currentTetromino = board->nextTetromino;
    board->nextTetromino = getNextTetromino(board);
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
            }
        }
    }

    drawGrid(board);
    drawBoardBorder(board);
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

char hasLost(Board* board) {
    return (board->currentTetromino->y == 0) && !canMove(board, 0, 0);
}

void setCellColor(Board * board, int x, int y, int r, int g, int b)
{
    board->gameBoard[y][x].color[0] = r;
    board->gameBoard[y][x].color[1] = g;
    board->gameBoard[y][x].color[2] = b;
}

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
