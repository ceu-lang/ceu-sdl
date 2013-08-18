// 119 -> 122 FPS

#include "SDL2/SDL.h"

typedef struct {
    SDL_Rect r;
    float    x;
    int      vel;
} Rect;

int main (int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win =
        SDL_CreateWindow("SDL 8", 500, 1300, 1024, 768, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren =
        SDL_CreateRenderer(win, -1, 0);

    Rect rs[10000];
    int i;
    for (i=0; i<10000; i++) {
        rs[i].r.w = 10;
        rs[i].r.h = 10;
        rs[i].r.y = rand()%750;

        rs[i].x   = rand()%930 + 50;
        rs[i].vel = 50 + rand()%20;
    }

    int old   = SDL_GetTicks();
    int start = SDL_GetTicks();
    int frames = 0;
    for (;;)
    {
        // calculate DT
        int now = SDL_GetTicks();
        int dt = now - old;
        old = now;

        if ((now-start) > 1000) {
            printf("FPS = %d\n", frames);
            frames = 0;
            start = now;
        }
        frames++;

        // update all RS
        for (i=0; i<10000; i++) {
            rs[i].x += ((float)rs[i].vel*dt)/1000;
        }

        // clear all
        SDL_SetRenderDrawColor(ren, 0,0,0,0xFF);
        SDL_Rect bg = { 0,0,1024,768 };
        SDL_RenderFillRect(ren, &bg);

        // draw all RS
        for (i=0; i<10000; i++) {
            rs[i].r.x = ((int)rs[i].x)%930 + 20;
            SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0xFF);
            SDL_RenderFillRect(ren, &rs[i].r);
        }

        // update screen
        SDL_RenderPresent(ren);
    }
}
