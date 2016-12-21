Data Abstractions
=================

SDL_Color
---------

```ceu
data SDL_Color with
    var u8 r;
    var u8 g;
    var u8 b;
    var u8 a;
end
```

SDL Reference: <https://wiki.libsdl.org/SDL_Color>

SDL_Point
---------

```ceu
data SDL_Point with
    var int x;
    var int y;
end
```

SDL Reference: <https://wiki.libsdl.org/SDL_Point>

SDL_Rect
--------

```ceu
data SDL_Rect with
    var int x;
    var int y;
    var int w;
    var int h;
end
```

SDL Reference: <https://wiki.libsdl.org/SDL_Rect>

SDL_Texture
-----------

```ceu
data SDL_Texture with
    var& _SDL_Texture tex;
    var  int          width;
    var  int          height;
end
```

SDL Reference: <https://wiki.libsdl.org/SDL_Texture>
