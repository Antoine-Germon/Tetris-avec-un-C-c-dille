#include "game.h"

void showMainMenu() {
    int buttonWidth = 350;
    int buttonHeight = 50;

    MenuButton soloButton = {
        .rect = {MULTIPLAYER_WINDOW_WIDTH / 2 - (buttonWidth / 2), 200, buttonWidth, buttonHeight},
        .text = "Jouer en solo",
        .baseColor = {70, 130, 180},
        .hoverColor = {100, 149, 237},
        .currentColor = {70, 130, 180},
        .hovered = false
    };

    MenuButton vsBotButton = {
        .rect = {MULTIPLAYER_WINDOW_WIDTH / 2 - (buttonWidth / 2), 300, buttonWidth, buttonHeight},
        .text = "Jouer contre IA",
        .baseColor = {34, 139, 34},
        .hoverColor = {60, 179, 113},
        .currentColor = {34, 139, 34},
        .hovered = false
    };

    MenuButton quitButton = {
        .rect = {MULTIPLAYER_WINDOW_WIDTH / 2 - (buttonWidth / 2), 400, buttonWidth, buttonHeight},
        .text = "Quitter",
        .baseColor = {232, 60, 60},
        .hoverColor = {230, 92, 92},
        .currentColor = {232, 60, 60},
        .hovered = false
    };

    bool running = true;
    SDL_Event event;
    int mouseX, mouseY;

    while (running) {
        SDL_GetMouseState(&mouseX, &mouseY);
        updateButtonHover(&soloButton, mouseX, mouseY);
        updateButtonHover(&vsBotButton, mouseX, mouseY);
        updateButtonHover(&quitButton, mouseX, mouseY);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return;
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (soloButton.hovered) {
                    SDL_SetWindowSize(pWindow, SINGLEPLAYER_WINDOW_WIDTH, WINDOW_HEIGHT);
                    singleGame();
                    return;
                } else if (vsBotButton.hovered) {
                    SDL_SetWindowSize(pWindow, MULTIPLAYER_WINDOW_WIDTH, WINDOW_HEIGHT);
                    botGame();
                    return;
                } else if (quitButton.hovered) {
                    exit(0);
                    return;
                }
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        drawButton(&soloButton);
        drawButton(&vsBotButton);
        drawButton(&quitButton);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
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

    int charWidth = 25;
    int charHeight = 25;
    int spacing = -5;

    int textX = button->rect.x + (button->rect.w - len * (charWidth + spacing)) / 2;
    int textY = button->rect.y + (button->rect.h - charHeight) / 2;

    //drawText(button->text, textX, textY, (SDL_Color){255, 255, 255});
    drawTextSpaced(button->text, textX, textY, (SDL_Color){255, 255, 255}, charWidth, charHeight, spacing);
}

void drawBlock(int x, int y, int color[])
{
    SDL_Rect outsideBlock = {
        x * BLOCK_SIZE + X_OFFSET,
        y * BLOCK_SIZE + Y_OFFSET,
        BLOCK_SIZE,
        BLOCK_SIZE
    };

    int r = color[0];
    int g = color[1];
    int b = color[2];

    SDL_SetRenderDrawColor(renderer, r - r * 0.25, g - g * 0.25, b - b * 0.25, 255);

    SDL_RenderFillRect(renderer, &outsideBlock);

    double offset = 0.6;

    SDL_Rect insideBlock = {
        x * BLOCK_SIZE + X_OFFSET + (BLOCK_SIZE * (1 - offset) / 2),
        y * BLOCK_SIZE + Y_OFFSET + (BLOCK_SIZE * (1 - offset) / 2),
        BLOCK_SIZE * offset,
        BLOCK_SIZE * offset
    };

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    SDL_RenderFillRect(renderer, &insideBlock);
}

void drawGrid(Board* board) {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Couleur gris foncé pour la grille

    // Lignes verticales
    for (int x = 0; x <= BOARD_WIDTH; x++) {
        SDL_RenderDrawLine(
            renderer,
            x * BLOCK_SIZE + X_OFFSET + (board->x * BLOCK_SIZE),            // x début
            Y_OFFSET + (board->y * BLOCK_SIZE),                             // y début
            x * BLOCK_SIZE + X_OFFSET + (board->x * BLOCK_SIZE),            // x fin
            BOARD_HEIGHT * BLOCK_SIZE + Y_OFFSET + (board->y * BLOCK_SIZE)  // y fin
        );
    }

    // Lignes horizontales
    for (int y = 0; y <= BOARD_HEIGHT; y++) {
        SDL_RenderDrawLine(
            renderer,
            X_OFFSET + (board->x * BLOCK_SIZE),                            // x début
            y * BLOCK_SIZE + Y_OFFSET + (board->y * BLOCK_SIZE),           // y début
            BOARD_WIDTH * BLOCK_SIZE + X_OFFSET + (board->x * BLOCK_SIZE), // x fin
            y * BLOCK_SIZE + Y_OFFSET + (board->y * BLOCK_SIZE)            // y fin
        );
    }
}

void drawBoardBorder(Board* board) {
    SDL_Rect borderRect = {
        X_OFFSET + (board->x * BLOCK_SIZE),  // x
        Y_OFFSET + (board->y * BLOCK_SIZE),  // y
        BOARD_WIDTH * BLOCK_SIZE,  // largeur totale du plateau
        BOARD_HEIGHT * BLOCK_SIZE  // hauteur totale du plateau
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc, par exemple

    SDL_RenderDrawRect(renderer, &borderRect); // Dessine un rectangle vide

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge

    int lineY = Y_OFFSET + (board->y + 4) * BLOCK_SIZE; // Ligne 4 en pixels
    int startX = X_OFFSET + (board->x * BLOCK_SIZE);
    int endX = startX + BOARD_WIDTH * BLOCK_SIZE;

    // Ligne en tirets : 5 pixels dessinés, 5 pixels d’espace
    for (int x = startX; x < endX; x += 10) {
        SDL_RenderDrawLine(renderer, x, lineY, x + 5, lineY);
    }
}

void drawPlayerMenu(int x, int y, int score, int level, Tetromino* next) {
    drawScore(x, y, score);
    drawLevel( x,  y, level);
    drawNextTetromino(x, y, next);
}
void drawLevel(int x, int y, int level) {
    char buffer[32];
    sprintf(buffer, "Niveau: %d", level);
    SDL_Color white = {255, 255, 255};
    drawTextSpaced(buffer, x, y+50, white, 20, 20, 5);
}
void drawScore(int x, int y, int score) {
    char buffer[32];
    sprintf(buffer, "Score: %d", score);

    SDL_Color white = {255, 255, 255};

    drawTextSpaced(buffer, x, y, white, 20, 20, 5);
}

void drawNextTetromino(int x, int y, Tetromino* next) {
    int previewBlockSize = BLOCK_SIZE / 2;
    int previewOffsetX = x;
    int previewOffsetY = y + 100;

    for (int i = 0; i < TETROMINO_SHAPE_BOX_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SHAPE_BOX_SIZE; j++) {
            if (next->shape[i][j]) {
                SDL_Rect rect = {
                    previewOffsetX + j * previewBlockSize,
                    previewOffsetY + i * previewBlockSize,
                    previewBlockSize,
                    previewBlockSize
                };

                SDL_SetRenderDrawColor(renderer, next->color[0], next->color[1], next->color[2], 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void showPauseScreen(RestartCallback restartFunction, int width) {
    int buttonWidth = 300;
    int buttonHeight = 50;

        MenuButton resumeButton = {
        .rect = {width / 2 - buttonWidth / 2, 300, buttonWidth, buttonHeight},
        .text = "Reprendre",
    .baseColor = {128, 128, 128},       
    .hoverColor = {169, 169, 169},     
    .currentColor = {128, 128, 128},
        .hovered = false
    };
    MenuButton replayButton = {
        .rect = {width / 2 - buttonWidth / 2, 400, buttonWidth, buttonHeight},
        .text = "Rejouer",
        .baseColor = {70, 130, 180},
        .hoverColor = {100, 149, 237},
        .currentColor = {70, 130, 180},
        .hovered = false
    };
    MenuButton menuButton = {
        .rect = {width / 2 - buttonWidth / 2, 500, buttonWidth, buttonHeight},
        .text = "Menu",
        .baseColor = {34, 139, 34},
        .hoverColor = {60, 179, 113},
        .currentColor = {34, 139, 34},
        .hovered = false
    };
    MenuButton quitButton = {
        .rect = {width / 2 - buttonWidth / 2, 600, buttonWidth, buttonHeight},
        .text = "Quitter",
        .baseColor = {232, 60, 60},
        .hoverColor = {230, 92, 92},
        .currentColor = {232, 60, 60},
        .hovered = false
    };

    bool running = true;
    SDL_Event event;
    int mouseX, mouseY;

    while (running) {
        SDL_GetMouseState(&mouseX, &mouseY);
        updateButtonHover(&resumeButton, mouseX, mouseY);
        updateButtonHover(&replayButton, mouseX, mouseY);
        updateButtonHover(&menuButton, mouseX, mouseY);
        updateButtonHover(&quitButton, mouseX, mouseY);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit(0);
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (replayButton.hovered) {
                    restartFunction(); 
                    return;
                } else if (quitButton.hovered) {
                    exit(0);
                }
                else if (menuButton.hovered) {
                    SDL_SetWindowSize(pWindow, MULTIPLAYER_WINDOW_WIDTH, WINDOW_HEIGHT);

                    showMainMenu();
                }
                else if (resumeButton.hovered) {
                    return;
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Texte "Game Over"
        drawTextSpaced("Pause", width / 2 - 75, 150, (SDL_Color){255, 0, 0}, 30, 30, 2);
        drawButton(&resumeButton);

        drawButton(&replayButton);
        drawButton(&quitButton);
        drawButton(&menuButton);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
void showGameOverScreen(const char* text, RestartCallback restartFunction, int width) {
    int buttonWidth = 300;
    int buttonHeight = 50;

    MenuButton replayButton = {
        .rect = {width / 2 - buttonWidth / 2, 300, buttonWidth, buttonHeight},
        .text = "Rejouer",
        .baseColor = {70, 130, 180},
        .hoverColor = {100, 149, 237},
        .currentColor = {70, 130, 180},
        .hovered = false
    };
    MenuButton menuButton = {
        .rect = {width / 2 - buttonWidth / 2, 400, buttonWidth, buttonHeight},
        .text = "Menu",
        .baseColor = {34, 139, 34},
        .hoverColor = {60, 179, 113},
        .currentColor = {34, 139, 34},
        .hovered = false
    };
    MenuButton quitButton = {
        .rect = {width / 2 - buttonWidth / 2, 500, buttonWidth, buttonHeight},
        .text = "Quitter",
        .baseColor = {232, 60, 60},
        .hoverColor = {230, 92, 92},
        .currentColor = {232, 60, 60},
        .hovered = false
    };

    bool running = true;
    SDL_Event event;
    int mouseX, mouseY;

    while (running) {
        SDL_GetMouseState(&mouseX, &mouseY);
        updateButtonHover(&replayButton, mouseX, mouseY);
        updateButtonHover(&menuButton, mouseX, mouseY);
        updateButtonHover(&quitButton, mouseX, mouseY);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit(0);
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (replayButton.hovered) {
                    restartFunction(); 
                    return;
                } else if (quitButton.hovered) {
                    exit(0);
                }
                else if (menuButton.hovered) {
                    SDL_SetWindowSize(pWindow, MULTIPLAYER_WINDOW_WIDTH, WINDOW_HEIGHT);

                    showMainMenu();
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Texte "Game Over"
        drawTextSpaced(text, width / 2 - 150, 150, (SDL_Color){255, 0, 0}, 30, 30, 2);

        drawButton(&replayButton);
        drawButton(&quitButton);
        drawButton(&menuButton);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}