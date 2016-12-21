## Frame

The *frame-based mode* of Céu-SDL continually waits for SDL input events in an
infinite loop.
The waiting period is limited to the next frame period:

```c
int main (void) {
    ceu_start();
    while (<program-is-running>) {
        int timeout = (<program-has-pending-async>) ?
                        0 : MIN(<next-timer-in-ms>,<next-frame-in-ms>);
        if (<next-frame-expired>) {
            ceu_input(CEU_INPUT_SDL_DT, CEU_SDL_MODE_FRAME);
        }
        if (SDL_WaitEventTimeout(<...>, timeout);
            ceu_input(CEU_INPUT__*, <...>);     /* input SDL_QUIT, SDL_KEY*, SDL_MOUSE*, etc */
        }
        ceu_input(CEU_INPUT__NONE, <time>);     /* input async and timer */
        ceu_input(CEU_INPUT_SDL_REDRAW, <...>); /* input SDL_REDRAW after every input */
    }
    ceu_stop();
    return <program-escape-value>;
}
```

The inputs are polled on each loop iteration and changes are notified to the
Céu application through `ceu_input` calls.

The frame-based mode also provides the `CEU_SDL_DT` input, which is notified
after each frame period expires.
