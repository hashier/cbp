#include <SDL/SDL.h>

// tested with
//    gcc -lSDL main.c

SDL_Surface *screen;

void main (void) {
    int x, y;

    SDL_Init(0);
    screen = SDL_SetVideoMode(800, 600, 32, 0);

    for ( y = 0; y < 600; ++y) {
        for ( x = 0; x < 800; ++x) {
            (( unsigned int*) (screen->pixels)) [x + 800 * y] = x + y;
        }
    }

    SDL_Flip(screen);

    while (1);
}
