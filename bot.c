#include "bot.h"
#include "game.h"

Cell** copyGameBoard(Cell** original) {
    Cell** copy = malloc(BOARD_HEIGHT * sizeof(Cell*));
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        copy[i] = malloc(BOARD_WIDTH * sizeof(Cell));
        for (int j = 0; j < BOARD_WIDTH; j++) {
            copy[i][j] = original[i][j];
        }
    }
    return copy;
}

void freeGameBoard(Cell** board) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        free(board[i]);
    }
    free(board);
}

int evaluateGameBoard(Cell** gameBoard) {
    int heights[BOARD_WIDTH] = {0};
    int totalHeight = 0;
    int maxHeight = 0;
    int holes = 0;
    int bumpiness = 0;
    int completeLines = 0;

    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (gameBoard[y][x].occupied) {
                heights[x] = BOARD_HEIGHT - y;
                break;
            }
        }
        totalHeight += heights[x];
        if (heights[x] > maxHeight)
            maxHeight = heights[x];
    }

    for (int x = 0; x < BOARD_WIDTH; x++) {
        bool blockSeen = false;
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (gameBoard[y][x].occupied) {
                blockSeen = true;
            } else if (blockSeen) {
                holes++;
            }
        }
    }

    for (int x = 0; x < BOARD_WIDTH - 1; x++) {
        bumpiness += abs(heights[x] - heights[x + 1]);
    }

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!gameBoard[y][x].occupied) {
                full = false;
                break;
            }
        }
        if (full) completeLines++;
    }

    int score =
        -5 * totalHeight +
         20 * completeLines +
        -3 * holes +
        -1 * bumpiness;

    return score;
}


// 0=left, 1=right, 2=down, 3=rotate
int getBestMove(Board *board) {
    int bestScore = INT32_MIN; // on définit un high score
    int bestX = board->currentTetromino->x;
    int bestRotation = 0;

    for (int rot = 0; rot < 4; rot++) {
        Tetromino* testTetromino = malloc(sizeof(Tetromino));
        memcpy(testTetromino, board->currentTetromino, sizeof(Tetromino));

        for (int r = 0; r < rot; r++) {
            Tetromino* rotated = rotateTetrominoLeft(board);
            if (rotated) {
                free(testTetromino);
                testTetromino = rotated;
            }
        }

        for (int x = -2; x < BOARD_WIDTH; x++) {
            Tetromino testPiece = *testTetromino;
            testPiece.x = x;
            testPiece.y = 0;

            if (!isValidPosition(board, &testPiece)) continue;

            // On drop la pièce tout en bas du plateau
            while (isValidPosition(board, &testPiece)) {
                testPiece.y += 1;
            }
            testPiece.y -= 1;

            // On crée une copie du gameboard
            Cell** testBoard = copyGameBoard(board->gameBoard);

            for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++) {
                for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++) {
                    if (testPiece.shape[i][j]) {
                        int bx = testPiece.x + j;
                        int by = testPiece.y + i;
                        if (bx >= 0 && bx < BOARD_WIDTH && by >= 0 && by < BOARD_HEIGHT) {
                            testBoard[by][bx].occupied = 1;
                        }
                    }
                }
            }

            int score = evaluateGameBoard(testBoard);
            if (score > bestScore) {
                bestScore = score;
                bestX = x;
                bestRotation = rot;
            }

            freeGameBoard(testBoard);
        }
        free(testTetromino);
    }

    // Decide next move to reach bestX and bestRotation
    int curX = board->currentTetromino->x;
    if (bestRotation > 0)
        return 3;  // rotate
    else if (curX < bestX)
        return 1;  // move right
    else if (curX > bestX)
        return 0;  // move left
    else
        return 2;  // drop
}