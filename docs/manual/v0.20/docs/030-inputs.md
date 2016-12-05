Input Events
============

Céu-SDL provides mappings to all SDL events:

<https://wiki.libsdl.org/SDL_Event>

`TODO: not all inputs have been mapped`

In addition, .


SDL_REDRAW
----------

Declaration:

```
input _SDL_Renderer&&&& SDL_REDRAW;
```

Occurs in the end of every loop iteration in all [modes of operation](#TODO).

`TODO: payload`

SDL_DT
------

Declaration:

```
input int SDL_DT;
```

Occurs in the beginning of every loop iteration in the [frame-based](#TODO)
[mode of operation](#TODO).

The input value of type `int` is always equal to [`CEU_SDL_MODE_FRAME`](#TODO).

SDL_QUIT
--------

Declaration:

```
input void SDL_QUIT;
```

SDL Reference:

<https://wiki.libsdl.org/SDL_QuitEvent>

SDL_KEYDOWN
-----------

Signature:

```
input _SDL_KeyboardEvent&& SDL_KEYDOWN;
```

SDL Reference:

<https://wiki.libsdl.org/SDL_KeyboardEvent>

SDL_KEYUP
---------

Signature:

```
input _SDL_KeyboardEvent&& SDL_KEYUP;
```

SDL Reference:

<https://wiki.libsdl.org/SDL_KeyboardEvent>

SDL_MOUSEBUTTONDOWN
-------------------

Signature:

```
input _SDL_MouseButtonEvent&& SDL_MOUSEBUTTONDOWN;
```

SDL Reference:

<https://wiki.libsdl.org/SDL_MouseButtonEvent>

SDL_MOUSEBUTTONUP
-------------------

Signature:

```
input _SDL_MouseButtonEvent&& SDL_MOUSEBUTTONUP;
```

SDL Reference:

<https://wiki.libsdl.org/SDL_MouseButtonEvent>

SDL_MOUSEMOTION
---------------

Signature:

```
input _SDL_MouseMotionEvent&& SDL_MOUSEMOTION;
```

SDL Reference:

<https://wiki.libsdl.org/SDL_MouseMotionEvent>
