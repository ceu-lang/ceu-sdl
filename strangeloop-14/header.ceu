#ifndef ALL

#include "sdl.ceu"
#include "sdl-gfx.ceu"

input void SDL_QUIT;
input void SDL_REDRAW;
input int SDL_DT;
input _SDL_MouseButtonEvent&& SDL_MOUSEBUTTONDOWN;

native do
    SDL_Renderer* REN = NULL;
    int REN_W, REN_H;
    ##define ID(x) x
end
native @pure _ID();

#ifdef CEU_TIMEMACHINE
global do
#endif

var _SDL_Window&? win;
    finalize
        win = &_SDL_CreateWindow(HEADER_TITLE,
                _SDL_WINDOWPOS_UNDEFINED, _SDL_WINDOWPOS_UNDEFINED,
                640, 480, _SDL_WINDOW_SHOWN);
    with
        _SDL_DestroyWindow(&&win!);
    end

_SDL_GetWindowSize(&&win!, &&_REN_W, &&_REN_H);

finalize
    _REN = _SDL_CreateRenderer(&&win!, -1, 0);
with
    _SDL_DestroyRenderer(_REN);
end

#ifdef CEU_TIMEMACHINE
end
#endif

#else

#ifdef CEU_TIMEMACHINE
input bool OS_RESUME;
class Title with
do
    loop do
        _SDL_SetWindowTitle(_WIN, HEADER_TITLE);
        await OS_RESUME;
    end
end
var Title t;
#else
_SDL_SetWindowTitle(_WIN, HEADER_TITLE);
#endif

#endif
