CFLAGS = -DCEU_RUNTESTS -DCEU_DEBUG #-DCEU_DEBUG_TRAILS

all:
	ceu $(CEUFILE) --m4 --tp-word 4 --tp-pointer 4
	gcc -g -Os main.c $(CFLAGS) -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf \
		-o $(basename $(CEUFILE)).exe

clean:
	rm -f a.out *.exe _ceu_

.PHONY: all clean
