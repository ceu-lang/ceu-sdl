#CFLAGS = -DCEU_RUNTESTS -DCEU_DEBUG #-DCEU_DEBUG_TRAILS

all:
	ceu $(CEUFILE)
	#gcc main.c $(CFLAGS) -lSDL2
	gcc -g -Os main.c $(CFLAGS) -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lSDL2_gfx -lpthread \
		-o $(basename $(CEUFILE)).exe

ui-grid:
	ceu ui-grid.ceu
	gcc main.c $(CFLAGS) -lSDL2

ui-texture:
	ceu ui-texture.ceu
	gcc main.c $(CFLAGS) -lSDL2 -lSDL2_image -lSDL2_ttf

clean:
	find . -name "*.exe"  | xargs rm -f
	find . -name "*_m4"   | xargs rm -f
	find . -name "_ceu_*" | xargs rm -f

.PHONY: all clean
