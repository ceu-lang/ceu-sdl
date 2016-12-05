Modes of Operation
==================

A mode of operation specifies how Céu-SDL captures events from the environment
(e.g., key presses) and redirects them to the Céu application.

Céu-SDL supports *polling*, *waiting*, and *frame-based* modes of operation.

Each mode of operation uses a different compilation flag:

```
$ make CEU_ARGS="-DCEU_SDL_MODE_POLLING"  CEU_SRC=<path-to-ceu-application>
$ make CEU_ARGS="-DCEU_SDL_MODE_WAITING"  CEU_SRC=<path-to-ceu-application>
$ make CEU_ARGS="-DCEU_SDL_MODE_FRAME=33" CEU_SRC=<path-to-ceu-application>
```

The frame-based mode also requires the desired period of the frame in
milliseconds.

The frame-based mode is the default mode of operation and is preset to 33
milliseconds.

The modes of operation are implemented in `C` and are part of Céu-SDL.
Each mode is described in pseudo-code as follows.

Polling Mode
------------

The *polling mode* of Céu-SDL continually checks for SDL input events in an
infinite loop:

```c
int main (void) {
    ceu_start();
    while (<program-is-running>) {
        if (SDL_PollEvent(<...>)) {
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

The polling mode uses *100%* of the CPU time.

Waiting Mode
------------

The *waiting mode* of Céu-SDL continually waits for SDL input events in an
infinite loop:

```c
int main (void) {
    ceu_start();
    while (<program-is-running>) {
        int timeout = (<program-has-pending-async>) ? 0 : <next-timer-in-ms>;
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

Frame-Based Mode
----------------

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
