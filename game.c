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
                    moveTetromino(playerBoard, -1, 0, NULL);
                    break;
                case SDLK_RIGHT:
                    moveTetromino(playerBoard, 1, 0, NULL);
                    break;
                case SDLK_DOWN:
                    moveTetromino(playerBoard, 0, 1, NULL);
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
                    showPauseScreen(singleGame, SINGLEPLAYER_WINDOW_WIDTH);
                    break;
                default:
                    break;
                }
            }
        }

        if (hasLost(playerBoard)) {
            showGameOverScreen("GAME OVER", singleGame, SINGLEPLAYER_WINDOW_WIDTH);
            break;
        }
        int fallSpeed = 500 - (playerBoard->level - 1) * 100; 
        if (fallSpeed < 100) fallSpeed = 100;
        if (SDL_GetTicks() - lastFallTime > fallSpeed)
        {
            moveTetromino(playerBoard, 0, 1, NULL);

            lastFallTime = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);    
        draw(playerBoard);

        drawPlayerMenu(X_OFFSET + BOARD_WIDTH * BLOCK_SIZE + 20, Y_OFFSET, playerBoard->score, playerBoard->level, playerBoard->nextTetromino);
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
                    moveTetromino(playerBoard, -1, 0, computerBoard);
                    break;
                case SDLK_RIGHT:
                    moveTetromino(playerBoard, 1, 0, computerBoard);
                    break;
                case SDLK_DOWN:
                    moveTetromino(playerBoard, 0, 1, computerBoard);
                    break;
                case SDLK_r:
                {
                   Tetromino *rotated = rotateTetrominoLeft(playerBoard);
                    if (rotated != NULL) {
                        memcpy(playerBoard->currentTetromino, rotated, sizeof(Tetromino));
                        free(rotated);
                    }
                    break;
                }
                case SDLK_ESCAPE:
                    showPauseScreen(botGame, MULTIPLAYER_WINDOW_WIDTH);
                    break;
                default:
                    break;
                }
            }
        }

        if (hasLost(playerBoard)) {
            showGameOverScreen("YOU LOSE", botGame, MULTIPLAYER_WINDOW_WIDTH);
            break;
        }

        if (hasLost(computerBoard)) {
            showGameOverScreen("YOU WIN", botGame, MULTIPLAYER_WINDOW_WIDTH);
            break;
        }

        if (SDL_GetTicks() - lastFallTime > 10) {
            int move = getBestMove(computerBoard);

            switch (move) {
                case 0:
                    moveTetromino(computerBoard, -1, 0, playerBoard);
                    break;
                case 1:
                    moveTetromino(computerBoard, 1, 0, playerBoard);
                    break;
                case 2:
                    moveTetromino(computerBoard, 0, 1, playerBoard);
                    break;
                case 3:
                {
                   Tetromino *rotated = rotateTetrominoLeft(computerBoard);
                    if (rotated != NULL) {
                        free(computerBoard->currentTetromino);
                        computerBoard->currentTetromino = rotated;
                    }
                    break;
                }
            }
        }

        if (SDL_GetTicks() - lastFallTime > 500)
        {
            moveTetromino(playerBoard, 0, 1, computerBoard);
            moveTetromino(computerBoard, 0, 1, playerBoard);
            
            lastFallTime = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);    

        draw(playerBoard);
        draw(computerBoard);

        drawPlayerMenu(X_OFFSET + BOARD_WIDTH * BLOCK_SIZE + 20, Y_OFFSET, playerBoard->score, playerBoard->level,playerBoard->nextTetromino);
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

                if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
                    return 0;
                }

                if (board->gameBoard[y][x].occupied) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int placeTetromino(Board * board)
{
    int checkedLines[BOARD_HEIGHT] = {0};
    int nbOfCheckedLines = 0;
    int nbClearedLine = 0;
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
        if (lineFull){
            clearLine(board, row);
            nbClearedLine++;
            board->linesCleared++;
            if (board->linesCleared % 5 == 0) {
                board->level++;
            }
        } 
    }

    if(nbClearedLine){
        board->score += BASE_SCORE*(1+2*(nbClearedLine-1));
    }
   
    free(board->currentTetromino);
    board->currentTetromino = board->nextTetromino;
    board->nextTetromino = getNextTetromino(board);
     if (board->currentTetromino == NULL || board->nextTetromino == NULL) {
        printf("ERREUR: Impossible de générer le prochain Tetromino.\n");
        exit(EXIT_FAILURE);
    }
    board->currentTetromino->x = 3;
    board->currentTetromino->y = 0;

    return nbClearedLine;
}

void addGarbageLine(Board* board, int numberOfRows) {
    for (int i = 0; i < numberOfRows; i++) {
        // on décale toutes les lignes vers le haut
        for (int y = 0; y < BOARD_HEIGHT - 1; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board->gameBoard[y][x] = board->gameBoard[y + 1][x];
            }
        }

        // on gènère une ligne pleine avecjuste une seule case vide
        int holeIndex = rand() % BOARD_WIDTH;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            board->gameBoard[BOARD_HEIGHT - 1][x].occupied = 1;
            if (x == holeIndex) {
                board->gameBoard[BOARD_HEIGHT - 1][x].occupied = 0;
            }

            board->gameBoard[BOARD_HEIGHT - 1][x].color[0] = 100;
            board->gameBoard[BOARD_HEIGHT - 1][x].color[1] = 100;
            board->gameBoard[BOARD_HEIGHT - 1][x].color[2] = 100;
        }

        bool overlap = false;
        Tetromino *t = board->currentTetromino;
        for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++) {
            for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++) {
                if (t->shape[i][j]) {
                    int x = t->x + j;
                    int y = t->y + i;
                    if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                        if (board->gameBoard[y][x].occupied) {
                            overlap = true;
                        }
                    }
                }
            }
        }
        if (overlap) {
            t->y -= 1;
        }
    }
}

void moveTetromino(Board * board, int dx, int dy, Board* opponent)
{
    if (canMove(board, dx, dy))
    {
        board->currentTetromino->x += dx;
        board->currentTetromino->y += dy;
    }
    else if (dy > 0)
    {
        int linesCleared = placeTetromino(board);
        if (opponent && linesCleared > 0) {
            addGarbageLine(opponent, linesCleared);
        }
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
    for (int i = row; i > 0; i--) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board->gameBoard[i][j] = board->gameBoard[i - 1][j];
        }
    }

   for (int j = 0; j < BOARD_WIDTH; j++) {
        board->gameBoard[0][j].occupied = 0;
        setCellColor(board, j, 0, 0, 0, 0);
    }
}

char hasLost(Board* board) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board->gameBoard[i][j].occupied) return true;
        }
    }
    return false;
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
