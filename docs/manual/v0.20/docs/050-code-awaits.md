Code/Awaits
===========

SDL_go
------

Creates a SDL window and provides a renderer to the application.

`SDL_go` terminates when [`SDL_QUIT`](#TODO) occurs.

```
code/await SDL_go (var _char&& title, var int width, var int height, var SDL_Color? bg)
                    -> (var& _SDL_Renderer ren)
                        -> void
```

- Parameters
    - `title`:  the title  of the window
    - `width`:  the width  of the window in pixels
    - `height`: the height of the window in pixels
    - `bg`:     the background color of the window (optional)
- Initialization
    - `ren`:    alias to managed renderer
- Return
    - terminates on `SDL_QUIT` and returns no value

`SDL_go` performs a number of initializations to put Céu-SDL in an usable state:
- inititalizes SDL
- creates a window
- creates a renderer
- initializes TTF
- initializes audio

All allocated resources are released on termination.

Example:

```
#include "sdl/sdl.ceu"

var& _SDL_Renderer ren; ;
watching SDL_go("Title", 300,300, SDL_Color(0xFF,0xFF,0x00,0xFF)) -> (&ren) do
    var SDL_Rect rect = val SDL_Rect(100,100 , 100,100);
    every SDL_REDRAW do
        _SDL_SetRenderDrawColor(&&ren, 0xFF,0x00,0x00,0xFF);
        _SDL_RenderFillRect(&&ren, (&&rect as _SDL_Rect&&));
    end
end

escape 0;
```

Draws a *100x100* red rectangle centered in a *300x300* yellow window.

SDL_open_texture
----------------

```
code/await SDL_open_texture (var& _SDL_Renderer ren, var _char&& path)
                                -> (var& SDL_Texture tex)
                                    -> FOREVER
```

SDL_open_font
-------------

```
code/await SDL_open_font (var _char&& path, var int size)
                            -> (var& _TTF_Font font)
                                -> FOREVER
```

SDL_new_text
------------

```
code/await SDL_new_text (var& _SDL_Renderer ren, var& _TTF_Font font,
                         var _char&& text, var SDL_Color color)
                            -> (var& SDL_Texture tex)
                                -> FOREVER
```

SDL_open_sound
--------------

```
code/await SDL_open_sound (var _char&& path)
                            -> (var& _Mix_Chunk sound)
                                -> FOREVER
```
