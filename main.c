#include <stdio.h>

#ifdef __ANDROID__
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengles.h"
#define printf(args...)     __android_log_print(4, "SDL", ## args);
#define fprintf(x, args...) __android_log_print(4, "SDL", ## args);
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_opengl.h"
#endif

#include <assert.h>
#include <stdint.h>
typedef int64_t  s64;
typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t    s8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t   u8;

#include "_ceu_defs.h"

s32 WCLOCK_nxt;

#ifndef IN_SDL_DT
#define ceu_out_wclock(us) WCLOCK_nxt = us;
#endif

#ifdef CEU_ASYNCS
int ASYNC_nxt = 0;
#define ceu_out_async(v) ASYNC_nxt = v;
#endif

#include "_ceu_code.cceu"

#ifdef __ANDROID__
int SDL_main (int argc, char *argv[])
#else
int main (int argc, char *argv[])
#endif
{
    int ret;

    SDL_Init(SDL_INIT_EVERYTHING);

#ifdef IN_SDL_DT
    WCLOCK_nxt = 20000;
#else
    WCLOCK_nxt = CEU_WCLOCK_NONE;
#endif

#ifdef CEU_WCLOCKS
    u32 old = SDL_GetTicks();
#endif

    if (ceu_go_init(&ret))
        goto END;

// TODO: push START into queue
#ifdef IN_START
    if (ceu_go_event(&ret, IN_START, NULL))
        goto END;
#endif
#ifdef IN_SDL_REDRAW
    if (ceu_go_event(&ret, IN_SDL_REDRAW, NULL))
        goto END;
#endif

    SDL_Event evt;
    for (;;)
    {
#ifndef SDL_SIMUL

        s32 tm = -1;
#ifdef CEU_WCLOCKS
        if (WCLOCK_nxt != CEU_WCLOCK_NONE)
            tm = WCLOCK_nxt / 1000;
#endif
#ifdef CEU_ASYNCS
        if (ASYNC_nxt)
            tm = 0;
#endif

        int has = SDL_WaitEventTimeout(&evt, tm);
//printf("...\n");

#ifdef CEU_WCLOCKS
        u32 now = SDL_GetTicks();
        s32 dt = now - old;
        old = now;

        if (WCLOCK_nxt != CEU_WCLOCK_NONE) {
            s32 nxt;
            int s = ceu_go_wclock(&ret, 1000*dt, &nxt);
            if (s) goto END;
            while (nxt <= 0) {
                s = ceu_go_wclock(&ret, 0, &nxt);
                if (s) goto END;
            }
        }
#endif

#ifdef IN_SDL_DT
        if (ceu_go_event(&ret, IN_SDL_DT, &dt))
            goto END;
#endif

        // OTHER EVENTS
        if (has)
        {
            int s = 0;
//printf("EVT: %x\n", evt.type);
            switch (evt.type) {
#ifdef IN_SDL_QUIT
                case SDL_QUIT:
                    s = ceu_go_event(&ret, IN_SDL_QUIT, NULL);
                    break;
#endif
#ifdef IN_SDL_WINDOWEVENT
                case SDL_WINDOWEVENT:
                    s = ceu_go_event(&ret, IN_SDL_WINDOWEVENT, &evt);
                    break;
#endif
#ifdef IN_SDL_KEYDOWN
                case SDL_KEYDOWN:
                    s =  ceu_go_event(&ret, IN_SDL_KEYDOWN, &evt);
                    break;
#endif
#ifdef IN_SDL_KEYUP
                case SDL_KEYUP:
                    s = ceu_go_event(&ret, IN_SDL_KEYUP, &evt);
                    break;
#endif
#ifdef IN_SDL_TEXTINPUT
                case SDL_TEXTINPUT:
                    s = ceu_go_event(&ret, IN_SDL_TEXTINPUT, &evt);
                    break;
#endif
#ifdef IN_SDL_TEXTEDITING
                case SDL_TEXTEDITING:
                    s = ceu_go_event(&ret, IN_SDL_TEXTEDITING, &evt);
                    break;
#endif
#ifdef IN_SDL_MOUSEMOTION
                case SDL_MOUSEMOTION:
                    s = ceu_go_event(&ret, IN_SDL_MOUSEMOTION, &evt);
                    break;
#endif
#ifdef IN_SDL_MOUSEBUTTONDOWN
                case SDL_MOUSEBUTTONDOWN:
                    s = ceu_go_event(&ret, IN_SDL_MOUSEBUTTONDOWN, &evt);
                    break;
#endif
#ifdef IN_SDL_MOUSEBUTTONUP
                case SDL_MOUSEBUTTONUP:
                    s = ceu_go_event(&ret, IN_SDL_MOUSEBUTTONUP, &evt);
                    break;
#endif
#ifdef IN_SDL_FINGERDOWN
                case SDL_FINGERDOWN:
                    s = ceu_go_event(&ret, IN_SDL_FINGERDOWN, &evt);
                    break;
#endif
#ifdef IN_SDL_FINGERUP
                case SDL_FINGERUP:
                    s = ceu_go_event(&ret, IN_SDL_FINGERUP, &evt);
                    break;
#endif
            }
            if (s) goto END;
        }

#ifdef IN_SDL_REDRAW
        if (ceu_go_event(&ret, IN_SDL_REDRAW, NULL))
            goto END;
#endif

#endif  // SDL_SIMUL

#ifdef CEU_ASYNCS
        if (ASYNC_nxt) {
            if (ceu_go_async(&ret, NULL))
                goto END;
        }
#endif
    }
END:
    SDL_Quit();         // TODO: slow
    return ret;
}

