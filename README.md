SDL binding for Céu.

Céu is a reactive language that aims to offer a higher-level and safer 
alternative to C.

Try it online:

http://www.ceu-lang.org/

Source code:

https://github.com/fsantanna/ceu/

Join our chat:

https://gitter.im/fsantanna/ceu

[SDL](http://www.libsdl.org/) works in typical platforms (e.g., Windows, Mac, 
Linux, Android) and provides basic input & output functionality (e.g., timers, 
keyboard, mouse, display):

https://github.com/fsantanna/ceu-sdl/

# INSTALLATION

## Install required software:

```
$ sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-net-dev libsdl2-gfx-dev
```

(Assuming a Linux/Ubuntu machine.)

## Install Céu:

https://github.com/fsantanna/ceu/

## Run the examples

Edit the `Makefile` to point to your `ceu` directory and then run `make`:

```
$ gedit Makefile
$ make
```
