#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <time.h>

#include "include.h"
#include "dessincarre.h"
#include "input.h"


// creation de la window
SDL_Window* window = NULL;
// creation du render (corkisidequest j'ai fait la même ça marche, je touche pas)
SDL_Renderer* renderer = NULL;

// init classic tmtc
bool init() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

//cleaning up and close the SDL resources when game finished. It destroys the renderer and windwow after that quit the program. 
void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// gestion position fruit
void generateFruit() {
    fruitX = rand() % (WINDOW_WIDTH / GRID_SIZE);
    fruitY = rand() % (WINDOW_HEIGHT / GRID_SIZE);
}

// gestion mouvement
void mouvement() {
    for (int i = snakeLength; i > 0; --i) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    // les directions
    switch (direction) {
    case 0: // Haut
        snakeY[0] -= 1;
        break;
    case 1: // Droite
        snakeX[0] += 1;
        break;
    case 2: // Bas
        snakeY[0] += 1;
        break;
    case 3: // Gauche
        snakeX[0] -= 1;
        break;
    }
    // check si tu touches le bord
    if (snakeX[0] < 0 || snakeX[0] >= WINDOW_WIDTH / GRID_SIZE || snakeY[0] < 0 || snakeY[0] >= WINDOW_HEIGHT / GRID_SIZE) { 
        gameOver = true;
    }
    for (int i = 1; i < snakeLength; ++i) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = true;
        }
    }
    if (snakeX[0] == fruitX && snakeY[0] == fruitY) {
        score += 1; // bon je fais la variable du score mais on se sait je vais pas réussir à l'afficher 
        snakeLength++;
        generateFruit();
    }
}


void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snakeLength; ++i) {
        SDL_Rect rect = { snakeX[i] * GRID_SIZE, snakeY[i] * GRID_SIZE, GRID_SIZE, GRID_SIZE };
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect fruitRect = { fruitX * GRID_SIZE, fruitY * GRID_SIZE, GRID_SIZE, GRID_SIZE };
    SDL_RenderFillRect(renderer, &fruitRect);

    SDL_RenderPresent(renderer);
}

void reduction() {
    generateFruit();

    snakeX[0] = WINDOW_WIDTH / (2 * GRID_SIZE);
    snakeY[0] = WINDOW_HEIGHT / (2 * GRID_SIZE);
    snakeLength = 1;

    direction = -1;
    gameOver = false;
    score = 0;
}


int main(int argc, char* argv[]) {
    srand(time(NULL));

    // lance l'init et check si erreur
    if (!init()) {
        return 1;
    }

    draw(renderer);
    reduction();

    Uint32 lastTime = SDL_GetTicks();
    while (!gameOver) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - lastTime;

        if (deltaTime >= GAME_SPEED) {
            handleInput();
            mouvement();
            render();

            lastTime = currentTime;
        }
    }
    // clear tout
    close();

    return 0;
}