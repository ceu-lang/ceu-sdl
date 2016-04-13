SDL binding for Céu.

Céu is a reactive language that aims to offer a higher-level and safer 
alternative to C.

http://www.ceu-lang.org/

https://github.com/fsantanna/ceu/

[SDL](http://www.libsdl.org/) works in typical platforms (e.g., Windows, Mac, 
Linux, Android) and provides basic input & output functionality (e.g., timers, 
keyboard, mouse, display):

https://github.com/fsantanna/ceu-sdl/

Join our chat at https://gitter.im/fsantanna/ceu

# Summary

* [Quick Start](#quick-start)
* [Applications](#applications)

# Quick Start

## Install `sdl`

First, you need to install `sdl`:

```
# Ubuntu:
$ sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-net-dev libsdl2-gfx-dev

# Sources:
- SDL: http://www.libsdl.org/
```

## Install `ceu`

Then, you need to install `ceu`:

https://github.com/fsantanna/ceu/

## Download and Configure `ceu-sdl`

Then, you need to download and configure `ceu-sdl`:

```
$ git clone https://github.com/fsantanna/ceu-sdl/
$ cd ceu-sdl/
```

Edit the `Makefile` to point to your `ceu` directory:

```
$ vi Makefile
CEU_DIR ?= <path to "ceu" repository>  # EDIT THIS LINE
```

Run `make link` to properly point to your `ceu` repository:

```
$ make link
```

## Run the examples

Now, you are ready to run the examples:

```
$ make samples
for i in samples/*.ceu; do								\
		echo;									\
		echo -n "#####################################";		\
		echo    "#####################################";		\
		echo File: "$i";							\
		grep "#@" "$i" | cut -f2- -d" ";					\
		echo -n "#####################################";		\
		echo    "#####################################";		\
		echo -n "Press <enter> to start...";					\
		read _;									\
		if [ "$i" = "samples/test-03.ceu" ]; then				\
			make ARCH_DIR=arch/pthread SRC=$i all || exit 1;	\
		else									\
			make SRC=$i all || exit 1;					\
		fi;									\
		echo;									\
	done

##########################################################################
File: samples/blink-01.ceu
##########################################################################
Press <enter> to start...
```

# Applications

Ready-to-run applications that rely on `ceu-sdl`:

* Talk at "Strange Loop" and "Future Programming Workshop" (2014)
    * https://github.com/fsantanna/ceu-app-fpw/
* Talk at "Curry-On" (2015)
    * https://github.com/fsantanna/ceu-app-curryon/
* Paper at "REBLS" (2015)
    * https://github.com/fsantanna/ceu-app-rebls15/
* Video game "Rocks!"
    * https://github.com/droid-in-the-sky/rocks/
* Video game "Ultimate Tic-Tac-Toe"
    * https://github.com/droid-in-the-sky/ult-tic-tac-toe/
