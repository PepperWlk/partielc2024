#include "input.h"

// gestion input
void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        //si un event pour quitter happened alors bah ça quitte pas chercher midi à 14h
        if (event.type == SDL_QUIT) {
            gameOver = true;
        }
        //quel input
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (direction != 2)
                    direction = 0;
                break;
            case SDLK_DOWN:
                if (direction != 0)
                    direction = 2;
                break; 
            case SDLK_LEFT:
                if (direction != 1)
                    direction = 3;
                break;
            case SDLK_RIGHT:
                if (direction != 3)
                    direction = 1;
                break;
            }
        }
    }
}

