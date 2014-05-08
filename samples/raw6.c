// 119 -> 122 FPS

#include "SDL2/SDL.h"

typedef struct {
    SDL_Rect  r;
    float     x;
    int       vel;
    SDL_Color clr;
    int       timer;
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
        rs[i].r.w = 25;
        rs[i].r.h = 25;
        rs[i].r.y = rand()%750;

        rs[i].x   = rand()%930 + 50;
        rs[i].vel = 50 + rand()%20;

        rs[i].timer = 0;
    }

    int old   = SDL_GetTicks();
    int start = SDL_GetTicks();
    int frames = 0;

    SDL_Event evt;

    for (;;)
    {
        int has = SDL_WaitEventTimeout(&evt, 0);
        if (has) {
            if (evt.type == SDL_QUIT) {
                break;
            }
        }

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

        // h, w, clr
        for (i=0; i<10000; i++) {
            rs[i].timer += dt;
            if (rs[i].timer > 100) {
                rs[i].timer = 0;

                rs[i].r.h = rs[i].r.h + rand()%5 - 2;
                if (rs[i].r.h <= 0) {
                    rs[i].r.h = 0;
                }
                if (rs[i].r.h >= 50) {
                    rs[i].r.h = 50;
                }
                rs[i].r.w = rs[i].r.w + rand()%5 - 2;
                if (rs[i].r.w <= 0) {
                    rs[i].r.w = 0;
                }
                if (rs[i].r.w >= 50) {
                    rs[i].r.w = 50;
                }
                rs[i].clr.r += rand()%5 - 2;
                rs[i].clr.g += rand()%5 - 2;
                rs[i].clr.b += rand()%5 - 2;
            }
        }

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
            SDL_SetRenderDrawColor(ren, rs[i].clr.r, rs[i].clr.g, rs[i].clr.b, 0xFF);
            SDL_RenderFillRect(ren, &rs[i].r);
        }

        // update screen
        SDL_RenderPresent(ren);
    }

    return 0;
}
