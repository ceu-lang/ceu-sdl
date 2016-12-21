Code/Await Abstractions
=======================

SDL_Init
------

Initializes Céu-SDL, creates a SDL window, and provides a renderer to the
application.

`SDL_Init` terminates once [`SDL_QUIT`](#TODO) occurs.

```ceu
code/await SDL_Init (var _char&& title, var int width, var int height, var SDL_Color? bg)
                        -> (var& _SDL_Renderer ren)
                            -> void
```

- Parameters
    - `title`:  title  of the window
    - `width`:  width  of the window in pixels
    - `height`: height of the window in pixels
    - `bg`:     background color of the window (optional)
- Initialization
    - `ren`:    created renderer
- Return
    - terminates on `SDL_QUIT` and returns no value

`SDL_Init` performs a number of initializations on Céu-SDL:

- initializes SDL
- creates a window
- creates a renderer
- initializes the text and audio subsystems

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

*Note: all allocated SDL resources are automatically released on termination.*

SDL_Open_Image
--------------

Opens an image file into a new texture.

```ceu
code/await SDL_Open_Image (var& _SDL_Renderer ren, var _char&& path)
                            -> (var& SDL_Texture tex)
                                -> FOREVER
```

- Parameters
    - `ren`:  rendering context
    - `path`: path to the image
- Initialization
    - `tex`:  created [texture](#TODO)
- Return
    - never terminates naturally

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

*Note: all allocated SDL resources are automatically released on termination.*

SDL_Open_Font
-------------

Opens a `TTF` font file.

```ceu
code/await SDL_Open_Font (var _char&& path, var int size)
                            -> (var& _TTF_Font font)
                                -> FOREVER
```

- Parameters
    - `path`: path to the font
    - `size`: size of the font
- Initialization
    - `font`: created font
- Return
    - never terminates naturally

Example:

```ceu
#include "sdl/sdl.ceu"

var& _SDL_Renderer ren; ;
watching SDL_Init("Font 1", 220,60, SDL_Color(0xFF,0xFF,0x00,0xFF)) -> (&ren) do
    var& _TTF_Font font;
    spawn SDL_Open_Font("samples/Deutsch.ttf", 40) -> (&font);

    var& SDL_Texture txt;
    spawn SDL_New_Text(&ren, &font, "Hello World!", SDL_Color(0x00,0x00,0x00,0xFF))
            -> (&txt);
    var SDL_Rect rect = val SDL_Rect(10, 10, txt.width, txt.height);

    every SDL_REDRAW do
        _SDL_RenderCopy(&&ren, &&txt.tex, null, &&rect as _SDL_Rect&&);
    end
end

escape 0;
```

Draws a black `Hello World!` text in yellow window.

SDL References:
    [`_TTF_OpenFont`](#TODO),
    [`_TTF_WasInit`](#TODO),
    [`_TTF_CloseFont`](#TODO).

*Note: all allocated SDL resources are automatically released on termination.*

SDL_New_Text
------------

Writes a text into a new texture.

```ceu
code/await SDL_New_Text (var& _SDL_Renderer ren, var& _TTF_Font font, var _char&& text, var SDL_Color color)
                            -> (var& SDL_Texture tex)
                                -> FOREVER
```

- Parameters
    - `ren`:   rendering context
    - `font`:  text font
    - `text`:  text to write
    - `color`: text color
- Initialization
    - `tex`:  created [texture](#TODO)
- Return
    - never terminates naturally

Example:

```ceu
#include "sdl/sdl.ceu"

var& _SDL_Renderer ren; ;
watching SDL_Init("Font 1", 220,60, SDL_Color(0xFF,0xFF,0x00,0xFF)) -> (&ren) do
    var& _TTF_Font font;
    spawn SDL_Open_Font("font.ttf", 40) -> (&font);

    var& SDL_Texture txt;
    spawn SDL_New_Text(&ren, &font, "Hello World!", SDL_Color(0x00,0x00,0x00,0xFF))
            -> (&txt);
    var SDL_Rect rect = val SDL_Rect(10, 10, txt.width, txt.height);

    every SDL_REDRAW do
        _SDL_RenderCopy(&&ren, &&txt.tex, null, &&rect as _SDL_Rect&&);
    end
end

escape 0;
```

Draws a black `Hello World!` text in yellow window.

SDL References:
    [`_TTF_RenderText_Blended`],
    [`_SDL_FreeSurface`],
    [`_SDL_CreateTextureFromSurface`],
    [`_SDL_DestroyTexture`],
    [`_SDL_QueryTexture`].

*Note: all allocated SDL resources are automatically released on termination.*

SDL_Open_Sound
--------------

Opens a sound file.

```ceu
code/await SDL_Open_Sound (var _char&& path)
                            -> (var& _Mix_Chunk sound)
                                -> FOREVER
```

- Parameters
    - `path`: path to the sound
- Initialization
    - `sound`: created sound
- Return
    - never terminates naturally

Example:

```ceu
#include "sdl/sdl.ceu"

var SDL_Color bg = val SDL_Color(0x00,0x00,0x00,0xFF);

var& _SDL_Renderer ren;
watching SDL_Init("Sound 1", 10,10, bg) -> (&ren)
do
    var& _Mix_Chunk sound;
    spawn SDL_Open_Sound("sound.wav") -> (&sound);
    every 1s do
        _Mix_PlayChannel(-1, &&sound, 0);
    end
end

escape 0;
```

Plays `sound.wav` every second.

SDL References:
    [`_Mix_LoadWAV`](#TODO),
    [`_Mix_FreeChunk`](#TODO).

*Note: all allocated SDL resources are automatically released on termination.*
