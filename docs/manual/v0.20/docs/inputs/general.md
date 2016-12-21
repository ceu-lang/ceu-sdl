## General

### SDL_REDRAW

```ceu
input _SDL_Renderer&&&& SDL_REDRAW;
```

Occurs in the end of every loop iteration in all [modes of operation](#TODO).

`TODO: payload`

### SDL_DT

```ceu
input int SDL_DT;
```

Occurs in the beginning of every loop iteration in the [frame-based](#TODO)
[mode of operation](#TODO).

The input value of type `int` is always equal to [`CEU_SDL_MODE_FRAME`](#TODO).

### SDL_QUIT

```ceu
input void SDL_QUIT;
```

SDL Reference: <https://wiki.libsdl.org/SDL_QuitEvent>
