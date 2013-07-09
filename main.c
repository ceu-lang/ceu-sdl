#include <stdio.h>

// definitely lost: 3,285 bytes in 25 blocks


#ifdef __ANDROID__
#include "SDL.h"
#include "SDL_image.h"
//#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengles.h"
#define printf(args...)     __android_log_print(4, "SDL", ## args);
#define fprintf(x, args...) __android_log_print(4, "SDL", ## args);
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
//#include "SDL2/SDL_mixer.h"
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

int ret = 0;
int ret_val;
#define ceu_out_end(v) { ret=1; ret_val=v; }

#include "_ceu_defs.h"

s32 WCLOCK_nxt;

#ifndef CEU_IN_SDL_DT
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
    SDL_Init(SDL_INIT_EVERYTHING);

#ifdef CEU_IN_SDL_DT
    WCLOCK_nxt = 20000;
#else
    WCLOCK_nxt = CEU_WCLOCK_INACTIVE;
#endif

#if defined(CEU_WCLOCKS) || defined(CEU_IN_SDL_DT)
    u32 old = SDL_GetTicks();
#endif

    ceu_go_init();
    if (ret) goto END;

// TODO: push START into queue
#ifdef CEU_IN_START
    ceu_go_event(CEU_IN_START, NULL);
    if (ret) goto END;
#endif
#ifdef CEU_IN_SDL_REDRAW
    ceu_go_event(CEU_IN_SDL_REDRAW, NULL);
    if (ret) goto END;
#endif

    SDL_Event evt;
    for (;;)
    {
#ifndef SDL_SIMUL

#ifdef CEU_IN_SDL_DT
        s32 tm = 0;
#else
        s32 tm = -1;
#ifdef CEU_WCLOCKS
        if (WCLOCK_nxt != CEU_WCLOCK_INACTIVE)
            tm = WCLOCK_nxt / 1000;
#endif
#ifdef CEU_ASYNCS
        if (ASYNC_nxt)
            tm = 0;
#endif
#endif  // CEU_IN_SDL_DT

        int has = SDL_WaitEventTimeout(&evt, tm);

#if defined(CEU_WCLOCKS) || defined(CEU_IN_SDL_DT)
        u32 now = SDL_GetTicks();
        s32 dt = now - old;
        old = now;
#endif

        // redraw on wclock or any valid event (avoid undefined events)
        int redraw = 0;

#ifdef CEU_WCLOCKS
        if (WCLOCK_nxt != CEU_WCLOCK_INACTIVE) {
            redraw = WCLOCK_nxt <= 1000*dt;
            ceu_go_wclock(1000*dt);
            if (ret) goto END;
            while (WCLOCK_nxt <= 0) {
                ceu_go_wclock(0);
                if (ret) goto END;
            }
        }
#endif

#ifdef CEU_IN_SDL_DT
        ceu_go_event(CEU_IN_SDL_DT, (void*)dt);
        if (ret) goto END;
#endif

        // OTHER EVENTS
        if (has)
        {
            int handled = 1;        // =1 for defined events
//printf("EVT: %x\n", evt.type);
            switch (evt.type) {
#ifdef CEU_IN_SDL_QUIT
                case SDL_QUIT:
                    ceu_go_event(CEU_IN_SDL_QUIT, NULL);
                    break;
#endif
#ifdef CEU_IN_SDL_WINDOWEVENT
                case SDL_WINDOWEVENT:
                    ceu_go_event(CEU_IN_SDL_WINDOWEVENT, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_KEYDOWN
                case SDL_KEYDOWN:
                    ceu_go_event(CEU_IN_SDL_KEYDOWN, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_KEYUP
                case SDL_KEYUP:
                    ceu_go_event(CEU_IN_SDL_KEYUP, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_TEXTINPUT
                case SDL_TEXTINPUT:
                    ceu_go_event(CEU_IN_SDL_TEXTINPUT, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_TEXTEDITING
                case SDL_TEXTEDITING:
                    ceu_go_event(CEU_IN_SDL_TEXTEDITING, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_MOUSEMOTION
                case SDL_MOUSEMOTION:
                    ceu_go_event(CEU_IN_SDL_MOUSEMOTION, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_MOUSEBUTTONDOWN
                case SDL_MOUSEBUTTONDOWN:
                    ceu_go_event(CEU_IN_SDL_MOUSEBUTTONDOWN, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_MOUSEBUTTONUP
                case SDL_MOUSEBUTTONUP:
                    ceu_go_event(CEU_IN_SDL_MOUSEBUTTONUP, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_FINGERDOWN
                case SDL_FINGERDOWN:
                    ceu_go_event(CEU_IN_SDL_FINGERDOWN, &evt);
                    break;
#endif
#ifdef CEU_IN_SDL_FINGERUP
                case SDL_FINGERUP:
                    ceu_go_event(CEU_IN_SDL_FINGERUP, &evt);
                    break;
#endif
                default:
                    handled = 0;    // undefined event
            }
            if (ret) goto END;
            redraw = redraw || handled;
        }

#ifdef CEU_IN_SDL_REDRAW
        if (redraw) {
            ceu_go_event(CEU_IN_SDL_REDRAW, NULL);
            if (ret) goto END;
        }
#endif

#endif  // SDL_SIMUL

#ifdef CEU_ASYNCS
        if (ASYNC_nxt) {
            ceu_go_async(NULL);
            if (ret) goto END;
        }
#endif
    }
END:
    SDL_Quit();         // TODO: slow
    return ret_val;
}

