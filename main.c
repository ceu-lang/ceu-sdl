#include <stdio.h>

// definitely lost: 2,478 bytes in 17 blocks

#ifdef __ANDROID__
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
//#include "SDL_net.h"
//#include "SDL_opengles.h"
#ifdef DEBUG
#define printf(args...)     __android_log_print(4, "SDL", ## args);
#define fprintf(x, args...) __android_log_print(4, "SDL", ## args);
#else
#define printf(args...)
#define fprintf(x, args...)
#endif
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_opengl.h"
#endif

#include <assert.h>

#include "_ceu_app.h"

s32 WCLOCK_nxt;
#ifndef CEU_IN_SDL_DT
#define ceu_out_wclock_set(us) WCLOCK_nxt = us;
#endif

#ifdef CEU_ASYNCS
int ASYNC_nxt = 0;
#define ceu_out_async(v) ASYNC_nxt = v;
#endif

#ifdef __ANDROID__
#define SDL_MOTION_FLOOD_AVOID
#endif
#ifdef SDL_MOTION_FLOOD_AVOID
int FLOOD_FILTER (SDL_Event* evt, void* fingerId) {
    return evt->type == SDL_FINGERMOTION &&
           ((SDL_TouchFingerEvent*)evt)->fingerId == (int)fingerId;
}
#endif

#include "_ceu_app.c"

#ifdef __ANDROID__
int SDL_main (int argc, char *argv[])
#else
int main (int argc, char *argv[])
#endif
{
    int err = SDL_Init(SDL_INIT_EVERYTHING);
    if (err != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return err;
    }

    WCLOCK_nxt = CEU_WCLOCK_INACTIVE;
#if defined(CEU_WCLOCKS) || defined(CEU_IN_SDL_DT)
    u32 old = SDL_GetTicks();
#endif

#ifdef CEU_THREADS
    // just before executing CEU code
    CEU_THREADS_MUTEX_LOCK(&CEU.threads_mutex);
#endif

    char CEU_DATA[sizeof(CEU_Main)];
    tceu_app app;
        app.data = (tceu_org*) &CEU_DATA;
        app.init = &ceu_app_init;

    app.init(&app);    /* calls CEU_THREADS_MUTEX_LOCK() */
#ifdef CEU_RET
    if (! app.isAlive)
        goto END;
#endif

#ifdef CEU_IN_OS_START
    ceu_sys_go(&app, CEU_IN_OS_START, (tceu_evtp)NULL);
#ifdef CEU_RET
    if (! app.isAlive)
        goto END;
#endif
#endif

#ifdef CEU_IN_SDL_REDRAW
    ceu_sys_go(&app, CEU_IN_SDL_REDRAW, (tceu_evtp)NULL);
#ifdef CEU_RET
    if (! app.isAlive)
        goto END;
#endif
#endif

    SDL_Event evt;
#ifdef __ANDROID__
    int isPaused = 0;
#endif

    for (;;)
    {
#ifdef CEU_THREADS
        // unlock from INIT->START->REDRAW or last loop iteration
        CEU_THREADS_MUTEX_UNLOCK(&CEU.threads_mutex);
#endif

#ifndef SDL_SIMUL

        /*
         * With    SDL_DT, 'tm=0' (update as fast as possible).
         * Without SDL_DT, 'tm=?' respects the timers.
         */
#ifdef CEU_IN_SDL_DT
        s32 tm =  0;
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

        int has;
#ifdef __ANDROID__
        if (isPaused) {
            has = SDL_WaitEvent(&evt);
        } else
#endif
        {
            has = SDL_WaitEventTimeout(&evt, tm);
        }
//if (has)
    //printf("EVENT %x\n", evt.type);

#if defined(CEU_WCLOCKS) || defined(CEU_IN_SDL_DT)
        u32 now = SDL_GetTicks();
        s32 dt = now - old;
        old = now;
#endif

        // redraw on wclock|handled|DT
        // (avoids redrawing for undefined events)
        int redraw = 0;

#ifdef CEU_THREADS
        // just before executing CEU code
        CEU_THREADS_MUTEX_LOCK(&CEU.threads_mutex);
#endif

#ifdef __ANDROID__
        if (!isPaused)
#endif
        {
#ifdef CEU_WCLOCKS
#ifndef CEU_IN_SDL_DT
            if (WCLOCK_nxt != CEU_WCLOCK_INACTIVE)
            {
                redraw = WCLOCK_nxt <= 1000*dt;
#endif
                ceu_sys_go(&app, CEU_IN__WCLOCK, (tceu_evtp)(1000*dt));
#ifdef CEU_RET
                if (! app.isAlive)
                    goto END;
#endif

                while (WCLOCK_nxt <= 0) {
                    ceu_sys_go(&app, CEU_IN__WCLOCK, (tceu_evtp)0);
#ifdef CEU_RET
                    if (! app.isAlive)
                        goto END;
#endif
                }
#ifndef CEU_IN_SDL_DT
            }
#endif
#endif
#ifdef CEU_IN_SDL_DT
            ceu_sys_go(&app, CEU_IN_SDL_DT, (tceu_evtp)dt);
#ifdef CEU_RET
            if (! app.isAlive)
                goto END;
#endif
            redraw = 1;
#endif
        }

        // OTHER EVENTS
        if (has)
        {
            int handled = 1;        // =1 for defined events
            tceu_evtp evtp = (tceu_evtp)(void*)&evt;
//printf("EVT: %x\n", evt.type);
            switch (evt.type) {
#ifdef CEU_IN_SDL_QUIT
                case SDL_QUIT:
                    ceu_sys_go(&app, CEU_IN_SDL_QUIT, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_WINDOWEVENT
                case SDL_WINDOWEVENT:
                    ceu_sys_go(&app, CEU_IN_SDL_WINDOWEVENT, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_KEYDOWN
                case SDL_KEYDOWN:
                    ceu_sys_go(&app, CEU_IN_SDL_KEYDOWN, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_KEYUP
                case SDL_KEYUP:
                    ceu_sys_go(&app, CEU_IN_SDL_KEYUP, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_TEXTINPUT
                case SDL_TEXTINPUT:
                    ceu_sys_go(&app, CEU_IN_SDL_TEXTINPUT, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_TEXTEDITING
                case SDL_TEXTEDITING:
                    ceu_sys_go(&app, CEU_IN_SDL_TEXTEDITING, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_MOUSEMOTION
                case SDL_MOUSEMOTION:
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEMOTION, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_MOUSEBUTTONDOWN
                case SDL_MOUSEBUTTONDOWN:
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEBUTTONDOWN, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_MOUSEBUTTONUP
                case SDL_MOUSEBUTTONUP:
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEBUTTONUP, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_FINGERDOWN
                case SDL_FINGERDOWN:
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERDOWN, evtp);
                    break;
#endif
#ifdef CEU_IN_SDL_FINGERUP
                case SDL_FINGERUP:
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERUP, evtp);
                    break;
#endif

#if defined(CEU_IN_SDL_APP_WILLENTERBACKGROUND) || defined(__ANDROID__)
                case SDL_APP_WILLENTERBACKGROUND:
#ifdef __ANDROID__
                    // handle onPause/onResume
                    isPaused = 1;
#endif
#ifdef CEU_IN_SDL_APP_WILLENTERBACKGROUND
                    ceu_sys_go(&app, CEU_IN_SDL_APP_WILLENTERBACKGROUND, evtp);
#endif
                    break;
#endif
#if defined(CEU_IN_SDL_APP_WILLENTERFOREGROUND) || defined(__ANDROID__)
                case SDL_APP_WILLENTERFOREGROUND:
#ifdef __ANDROID__
                    // handle onPause/onResume
                    isPaused = 0;
                    old = SDL_GetTicks();   // ignores previous 'old' on resume
#endif
#ifdef CEU_IN_SDL_APP_WILLENTERFOREGROUND
                    ceu_sys_go(&app, CEU_IN_SDL_APP_WILLENTERFOREGROUND, evtp);
#endif
                    break;
#endif
#ifdef CEU_IN_SDL_FINGERMOTION
                case SDL_FINGERMOTION:
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERMOTION, evtp);
#ifdef SDL_MOTION_FLOOD_AVOID
                    // handle MOTION floods
                    SDL_FlushEventsFilter(FLOOD_FILTER,
                        (void*)((SDL_TouchFingerEvent*)&evt)->fingerId);
                    SDL_FlushEvents(SDL_DOLLARGESTURE, SDL_MULTIGESTURE);
#endif
                    break;
#endif
                default:
                    handled = 0;    // undefined event
            }
#ifdef CEU_RET
            if (! app.isAlive) goto END;
#endif
            redraw = redraw || handled;
        }

#ifdef CEU_IN_SDL_REDRAW
        if (redraw) {
            ceu_sys_go(&app, CEU_IN_SDL_REDRAW, (tceu_evtp)NULL);
#ifdef CEU_RET
            if (! app.isAlive)
                goto END;
#endif
        }
#endif

#endif  // SDL_SIMUL

#ifdef CEU_ASYNCS
        if (ASYNC_nxt) {
            ceu_sys_go(&app, CEU_IN__ASYNC, (tceu_evtp)NULL);
#ifdef CEU_RET
            if (! app.isAlive)
                goto END;
#endif
        }
#endif
    }
END:
#ifdef CEU_THREADS
    // only reachable if LOCKED
    CEU_THREADS_MUTEX_UNLOCK(&CEU.threads_mutex);
#endif
    SDL_Quit();         // TODO: slow
#ifdef CEU_RET
    return app.ret;
#else
    return 0;
#endif
}

