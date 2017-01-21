# Modes of Operation

A mode of operation specifies how Céu-SDL captures events from the environment
(e.g., key presses) and redirects them to the Céu application.

Céu-SDL supports *polling*, *waiting*, and *frame-based* modes of operation.

Each mode of operation uses a different compilation flag:

```
$ make CEU_ARGS="-DCEU_SDL_MODE_POLLING"  CEU_SRC=<path-to-ceu-application>
$ make CEU_ARGS="-DCEU_SDL_MODE_WAITING"  CEU_SRC=<path-to-ceu-application>
$ make CEU_ARGS="-DCEU_SDL_MODE_FRAME=33" CEU_SRC=<path-to-ceu-application>
```

The frame-based mode also requires the desired period of the frame (in
milliseconds).

The frame-based mode is the default mode of operation and is preset to 33
milliseconds.

The modes of operation are implemented in C and are part of Céu-SDL.
Each mode is described in pseudo-code as follows.

{!modes/polling.md!}

{!modes/waiting.md!}

{!modes/frame.md!}
