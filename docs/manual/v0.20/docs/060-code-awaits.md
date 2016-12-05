Code/Await Abstractions
=======================

SDL_Init
------

Creates a SDL window and provides a renderer to the application.

`SDL_Init` terminates when [`SDL_QUIT`](#TODO) occurs.

```ceu
code/await SDL_Init (var _char&& title, var int width, var int height, var SDL_Color? bg)
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

`SDL_Init` performs a number of initializations on Céu-SDL:

- initializes SDL
- creates a window
- creates a renderer
- initializes the text and audio subsystems

All allocated resources are released on termination.

If a `bg` is provided, `SDL_Init` fills the window background with the provided
color on every `SDL_REDRAW`.

Example:

```ceu
#include "sdl/sdl.ceu"

var& _SDL_Renderer ren; ;
watching SDL_Init("Rectangle", 300,300, SDL_Color(0xFF,0xFF,0x00,0xFF)) -> (&ren) do
    var SDL_Rect rect = val SDL_Rect(100,100 , 100,100);
    every SDL_REDRAW do
        _SDL_SetRenderDrawColor(&&ren, 0xFF,0x00,0x00,0xFF);
        _SDL_RenderFillRect(&&ren, (&&rect as _SDL_Rect&&));
    end
end

escape 0;
```

Draws a *100x100* red rectangle centered in a *300x300* yellow window.

SDL References:
    [`SDL_Init`](#TODO),
    [`SDL_Quit`](#TODO),
    [`SDL_CreateWindow`](#TODO),
    [`SDL_DestroyWindow`](#TODO),
    [`SDL_CreateRenderer`](#TODO),
    [`SDL_DestroyRenderer`](#TODO),
    [`TTF_Init`](#TODO),
    [`TTF_Quit`](#TODO),
    [`Mix_OpenAudio`](#TODO),
    [`Mix_CloseAudio`](#TODO),
    [`SDL_SetRenderDrawColor`](#TODO),
    [`SDL_RenderClear`](#TODO).

SDL_Open_Image
--------------

Opens an image into a texture and manages it.

```ceu
code/await SDL_Open_Image (var& _SDL_Renderer ren, var _char&& path)
                                -> (var& SDL_Texture tex)
                                    -> FOREVER
```

- Parameters
    - `ren`:  the rendering context
    - `path`: the path to the image
- Initialization
    - `tex`:  alias to the managed [texture](#TODO)
- Return
    - never terminates naturally

All allocated resources are released on termination.

Example:

```ceu
#include "sdl/sdl.ceu"

var& _SDL_Renderer ren; ;
watching SDL_Init("Image", 68,68, SDL_Color(0xFF,0xFF,0x00,0xFF)) -> (&ren) do
    var& SDL_Texture img;
    spawn SDL_Open_Image(&ren, "img.png") -> (&img);
    var SDL_Rect rect = val SDL_Rect(10,10 , img.width,img.height);
    every SDL_REDRAW do
        _SDL_RenderCopy(&&ren, &&img.tex, null, &&rect as _SDL_Rect&&);
    end
end

escape 0;
```

Draws `img.png` in a *68x68* yellow window.

SDL References:
    [`IMG_LoadTexture`](#TODO),
    [`SDL_DestroyTexture`](#TODO),
    [`SDL_QueryTexture`](#TODO).

SDL_Open_Font
-------------

```ceu
code/await SDL_Open_Font (var _char&& path, var int size)
                            -> (var& _TTF_Font font)
                                -> FOREVER
```

SDL_New_Text
------------

```ceu
code/await SDL_New_Rext (var& _SDL_Renderer ren, var& _TTF_Font font,
                         var _char&& text, var SDL_Color color)
                            -> (var& SDL_Texture tex)
                                -> FOREVER
```

SDL_Open_Sound
--------------

```ceu
code/await SDL_Open_Sound (var _char&& path)
                            -> (var& _Mix_Chunk sound)
                                -> FOREVER
```
