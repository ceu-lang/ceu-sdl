#include <stdio.h>

/*
#ifndef DEBUG
#define DEBUG
#endif
*/

#ifdef CEU_TIMEMACHINE
int CEU_TIMEMACHINE_ON = 0;
#else
#define CEU_TIMEMACHINE_ON 0
#endif

#define CEU_FPS 50

// definitely lost: 2,478 bytes in 17 blocks

#ifdef __ANDROID__
#include "SDL.h"
#include "SDL_image.h"
//#include "SDL_mixer.h"
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
#define ceu_out_wclock_set(us) WCLOCK_nxt = us;

#include "_ceu_app.c"

static char CEU_DATA[sizeof(CEU_Main)];

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
    u32 old = SDL_GetTicks();
    u32 fps_old = old;

#ifdef CEU_THREADS
    // just before executing CEU code
    CEU_THREADS_MUTEX_LOCK(&CEU.threads_mutex);
#endif

    tceu_app app;
        app.data = (tceu_org*) &CEU_DATA;
        app.init = &ceu_app_init;

    app.init(&app);    /* calls CEU_THREADS_MUTEX_LOCK() */
#ifdef CEU_RET
    if (! app.isAlive)
        goto END;
#endif

#ifdef CEU_IN_OS_START_
    ceu_sys_go(&app, CEU_IN_OS_START_, NULL);
#ifdef CEU_RET
    if (! app.isAlive)
        goto END;
#endif
#endif
#ifdef CEU_IN_OS_START
if (!CEU_TIMEMACHINE_ON) {
    ceu_sys_go(&app, CEU_IN_OS_START, NULL);
#ifdef CEU_RET
    if (! app.isAlive)
        goto END;
#endif
}
#endif

#ifdef CEU_IN_SDL_REDRAW_
    ceu_sys_go(&app, CEU_IN_SDL_REDRAW_, NULL);
#ifdef CEU_RET
    if (! app.isAlive)
        goto END;
#endif
#endif
#ifdef CEU_IN_SDL_REDRAW
if (!CEU_TIMEMACHINE_ON) {
    ceu_sys_go(&app, CEU_IN_SDL_REDRAW, NULL);
#ifdef CEU_RET
    if (! app.isAlive)
        goto END;
#endif
}
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

        /*
         * With    SDL_DT, 'tm=0' (update as fast as possible).
         * Without SDL_DT, 'tm=?' respects the timers.
         */
#if defined(CEU_IN_SDL_DT) || defined(CEU_IN_SDL_DT_)
#ifdef CEU_FPS
        s32 tm = (CEU_TIMEMACHINE_ON ? 0 : (1000/CEU_FPS));
#else
        s32 tm = 0;     // as fast as possible
#endif
#else
        s32 tm = -1;
#ifdef CEU_WCLOCKS
        if (WCLOCK_nxt != CEU_WCLOCK_INACTIVE)
            tm = WCLOCK_nxt / 1000;
#endif
#ifdef CEU_ASYNCS
        if (app.pendingAsyncs) {
            tm = 0;
        }
#endif
#endif  // CEU_IN_SDL_DT

        //SDL_EventState(SDL_FINGERMOTION, SDL_IGNORE);

        int has;
#ifdef __ANDROID__
        if (isPaused) {
            has = SDL_WaitEvent(&evt);
        } else
#endif
        {
            has = SDL_WaitEventTimeout(&evt, tm);
        }

/* TODO: o 1o faz mais sentido, mas so o 2o funciona! */
/*
        u32 now = SDL_GetTicks();
        while (now <= old) {
            now = SDL_GetTicks();
        }
        s32 dt = now - old;
        old = now;
*/
        u32 now = SDL_GetTicks();
        s32 dt_ms = now - old;
        s32 dt_us = dt_ms*1000;
        assert(dt_ms >= 0);
        old = now;


        // DT/WCLOCK/REDRAW respecting FPS (at most)
        int fps_ok = 1;
/*
        int fps_ok = !SDL_PollEvent(NULL);
        if (! fps_ok) {
            if (old >= fps_old+1000/CEU_FPS) {
                fps_old = old;
                fps_ok = 1;
            }
        }
*/

#ifdef CEU_THREADS
        // just before executing CEU code
        CEU_THREADS_MUTEX_LOCK(&CEU.threads_mutex);
#endif

#ifdef __ANDROID__
        if (!isPaused)
#endif
        if (fps_ok) {
#ifdef CEU_WCLOCKS
#if ! (defined(CEU_IN_SDL_DT) || defined(CEU_IN_SDL_DT_))
            if (WCLOCK_nxt != CEU_WCLOCK_INACTIVE)
            {
                //redraw = WCLOCK_nxt <= dt_us;
#endif
#ifdef CEU_TIMEMACHINE
//#ifdef CEU_IN__WCLOCK_ (TODO: always defined)
                ceu_sys_go(&app, CEU_IN__WCLOCK_, &dt_us);
#ifdef CEU_RET
                if (! app.isAlive)
                    goto END;
#endif
                while (WCLOCK_nxt <= 0) {
                    s32 dt_us = 0;
                    ceu_sys_go(&app, CEU_IN__WCLOCK_, &dt_us);
#ifdef CEU_RET
                    if (! app.isAlive)
                        goto END;
#endif
                }
#endif
if (!CEU_TIMEMACHINE_ON) {
                ceu_sys_go(&app, CEU_IN__WCLOCK, &dt_us);
#ifdef CEU_RET
                if (! app.isAlive)
                    goto END;
#endif
                while (WCLOCK_nxt <= 0) {
                    s32 dt_us = 0;
                    ceu_sys_go(&app, CEU_IN__WCLOCK, &dt_us);
#ifdef CEU_RET
                    if (! app.isAlive)
                        goto END;
#endif
                }
}
#if ! (defined(CEU_IN_SDL_DT) || defined(CEU_IN_SDL_DT_))
            }
#endif
#endif

#ifdef CEU_IN_SDL_DT_
            if (fps_ok) {
                ceu_sys_go(&app, CEU_IN_SDL_DT_, &dt_ms);
            }
#ifdef CEU_RET
            if (! app.isAlive)
                goto END;
#endif
            //redraw = 1;
#endif
#ifdef CEU_IN_SDL_DT
if (!CEU_TIMEMACHINE_ON) {
            if (fps_ok) {
                ceu_sys_go(&app, CEU_IN_SDL_DT, &dt_ms);
            }
#ifdef CEU_RET
            if (! app.isAlive)
                goto END;
#endif
            //redraw = 1;
}
#endif
        }

        // OTHER EVENTS
        if (has)
        {
            int handled = 1;        // =1 for defined events
            SDL_Event* evtp = &evt;
            switch (evt.type) {
                case SDL_QUIT:
#ifdef CEU_IN_SDL_QUIT_
                    ceu_sys_go(&app, CEU_IN_SDL_QUIT_, &evtp);
#endif
#ifdef CEU_IN_SDL_QUIT
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_QUIT, &evtp);
}
#endif
                    break;
                case SDL_WINDOWEVENT:
#ifdef CEU_IN_SDL_WINDOWEVENT_
                    ceu_sys_go(&app, CEU_IN_SDL_WINDOWEVENT_, &evtp);
#endif
#ifdef CEU_IN_SDL_WINDOWEVENT
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_WINDOWEVENT, &evtp);
}
#endif
                    break;
                case SDL_KEYDOWN:
#ifdef CEU_IN_SDL_KEYDOWN_
                    ceu_sys_go(&app, CEU_IN_SDL_KEYDOWN_, &evtp);
#endif
#ifdef CEU_IN_SDL_KEYDOWN
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_KEYDOWN, &evtp);
}
#endif
                    break;
                case SDL_KEYUP:
#ifdef CEU_IN_SDL_KEYUP_
                    ceu_sys_go(&app, CEU_IN_SDL_KEYUP_, &evtp);
#endif
#ifdef CEU_IN_SDL_KEYUP
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_KEYUP, &evtp);
}
#endif
                    break;
                case SDL_TEXTINPUT:
#ifdef CEU_IN_SDL_TEXTINPUT_
                    ceu_sys_go(&app, CEU_IN_SDL_TEXTINPUT_, &evtp);
#endif
#ifdef CEU_IN_SDL_TEXTINPUT
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_TEXTINPUT, &evtp);
}
#endif
                    break;
                case SDL_TEXTEDITING:
#ifdef CEU_IN_SDL_TEXTEDITING_
                    ceu_sys_go(&app, CEU_IN_SDL_TEXTEDITING_, &evtp);
#endif
#ifdef CEU_IN_SDL_TEXTEDITING
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_TEXTEDITING, &evtp);
}
#endif
                    break;
                case SDL_MOUSEMOTION:
#ifdef CEU_IN_SDL_MOUSEMOTION_
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEMOTION_, &evtp);
#endif
#ifdef CEU_IN_SDL_MOUSEMOTION
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEMOTION, &evtp);
}
#endif
                    break;
                case SDL_MOUSEBUTTONDOWN:
#ifdef CEU_IN_SDL_MOUSEBUTTONDOWN_
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEBUTTONDOWN_, &evtp);
#endif
#ifdef CEU_IN_SDL_MOUSEBUTTONDOWN
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEBUTTONDOWN, &evtp);
}
#endif
                    break;
                case SDL_MOUSEBUTTONUP:
#ifdef CEU_IN_SDL_MOUSEBUTTONUP_
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEBUTTONUP_, &evtp);
#endif
#ifdef CEU_IN_SDL_MOUSEBUTTONUP
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_MOUSEBUTTONUP, &evtp);
}
#endif
                    break;
                case SDL_FINGERDOWN:
#ifdef CEU_IN_SDL_FINGERDOWN_
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERDOWN_, &evtp);
#endif
#ifdef CEU_IN_SDL_FINGERDOWN
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERDOWN, &evtp);
}
#endif
                    break;
                case SDL_FINGERUP:
#ifdef CEU_IN_SDL_FINGERUP_
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERUP_, &evtp);
#endif
#ifdef CEU_IN_SDL_FINGERUP
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERUP, &evtp);
}
#endif
                    break;
                case SDL_FINGERMOTION:
#ifdef CEU_IN_SDL_FINGERMOTION_
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERMOTION_, &evtp);
#endif
#ifdef CEU_IN_SDL_FINGERMOTION
if (!CEU_TIMEMACHINE_ON) {
                    ceu_sys_go(&app, CEU_IN_SDL_FINGERMOTION, &evtp);
}
#endif
                    break;

/* TODO: "_" events */
#if defined(CEU_IN_SDL_APP_WILLENTERBACKGROUND) || defined(__ANDROID__)
                case SDL_APP_WILLENTERBACKGROUND:
#ifdef __ANDROID__
                    // handle onPause/onResume
                    isPaused = 1;
#endif
#ifdef CEU_IN_SDL_APP_WILLENTERBACKGROUND
                    ceu_sys_go(&app, CEU_IN_SDL_APP_WILLENTERBACKGROUND, &evtp);
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
                    ceu_sys_go(&app, CEU_IN_SDL_APP_WILLENTERFOREGROUND, &evtp);
#endif
                    break;
#endif
                default:
                    handled = 0;    // undefined event
            }
#ifdef CEU_RET
            if (! app.isAlive) goto END;
#endif
            //redraw = redraw || handled;
        }

#ifdef CEU_IN_SDL_REDRAW_
        //if (redraw && !SDL_PollEvent(NULL))
        if (fps_ok) {
            ceu_sys_go(&app, CEU_IN_SDL_REDRAW_, NULL);
#ifdef CEU_RET
            if (! app.isAlive)
                goto END;
#endif
        }
#endif
#ifdef CEU_IN_SDL_REDRAW
if (!CEU_TIMEMACHINE_ON) {
        //if (redraw && !SDL_PollEvent(NULL))
        if (fps_ok) {
            ceu_sys_go(&app, CEU_IN_SDL_REDRAW, NULL);
#ifdef CEU_RET
            if (! app.isAlive)
                goto END;
#endif
        }
}
#endif

/* TODO: "_" events */
#ifdef CEU_ASYNCS
        if (app.pendingAsyncs) {
            ceu_sys_go(&app, CEU_IN__ASYNC, NULL);
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

