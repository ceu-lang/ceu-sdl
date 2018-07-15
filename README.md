Céu-SDL supports the development of SDL applications in the programming
language Céu:

- Source Code:   https://github.com/ceu-lang/ceu-sdl/
- Documentation: http://ceu-lang.github.io/ceu-sdl/
- Chat:          https://gitter.im/fsantanna/ceu

[SDL](http://www.libsdl.org/) is a cross-platform software development library
designed to provide a hardware abstraction layer to computer multimedia
hardware components.

Céu is a reactive language that aims to offer a higher-level and safer
alternative to C:

- Home Page:   http://www.ceu-lang.org/
- Source code: https://github.com/ceu-lang/ceu/

Céu-SDL empowers the development of SDL applications with the following
extensions:

- Awaiting events in direct/sequential style.
  <!-- (e.g., timers, key presses, mouse motion, etc).-->
- Parallel lines of execution with
    - safe abortion;
    - deterministic behavior (in contrast with threads).
- Asynchronous loops for heavy computations.
- Seamless integration with standard SDL (e.g., `SDL_RenderFillRect`,
  `SDL_RenderPresent`, etc).

Install
=======

## Install SDL:

```
$ sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-net-dev libsdl2-gfx-dev
```

(Assuming a Linux/Ubuntu machine.)

## Install Céu:

https://github.com/ceu-lang/ceu/

## Clone Céu-SDL:

```
$ git clone https://github.com/ceu-lang/ceu-sdl
$ cd ceu-sdl/
$ git checkout v0.30
```

## Compile and Run

Edit the `Makefile` to point to your `ceu` directory and then run `make`:

```
$ gedit Makefile
$ make
```

If necessary, configure the variables in the `Makefile`.
The default example moves a rectangle on screen.

To compile and run another application, run `make` and set `CEU_SRC`:

```
$ make CEU_SRC=<path-to-ceu-application>
```

Examples
========

The `samples/` directory contains a number of examples.

To run all examples one after another, run `make samples`:

```
$ make samples
```

TODO
----

Applications
============

`TODO`

<!--
The Birds Tutorial
------------------
-->

- https://github.com/ceu-lang/ceu-sdl-birds
- https://github.com/ceu-lang/ceu-sdl-storm
