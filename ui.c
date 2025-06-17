#include "game.h"

void showMainMenu() {
    MenuButton soloButton = {
        .rect = {MULTIPLAYER_WINDOW_WIDTH / 2 - 250, 200, 500, 50},
        .text = "Jouer en solo",
        .baseColor = {70, 130, 180},
        .hoverColor = {100, 149, 237},
        .currentColor = {70, 130, 180},
        .hovered = false
    };

    MenuButton vsBotButton = {
        .rect = {MULTIPLAYER_WINDOW_WIDTH / 2 - 250, 300, 500, 50},
        .text = "Jouer contre IA",
        .baseColor = {34, 139, 34},
        .hoverColor = {60, 179, 113},
        .currentColor = {34, 139, 34},
        .hovered = false
    };

    MenuButton quitButton = {
        .rect = {MULTIPLAYER_WINDOW_WIDTH / 2 - 250, 400, 500, 50},
        .text = "quitter",
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
                    botGame();
                    return;
                } else if (quitButton.hovered) {
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

    int textX = button->rect.x + (button->rect.w - len * CHAR_WIDTH) / 2;
    int textY = button->rect.y + (button->rect.h - CHAR_HEIGHT) / 2;

    drawText(button->text, textX, textY, (SDL_Color){255, 255, 255});
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
