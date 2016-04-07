C_FLAGS += -DCEU_DEBUG #-DCEU_DEBUG_TRAILS
#C_FLAGS = -DSDL_MATH

C_FLAGS += -lpthread -lm \
		  -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lSDL2_gfx

sim:
	ceu --timemachine --cpp-args "-I . -DCEUFILE=$(CEUFILE)" sim.ceu
	#gcc main.c $(C_FLAGS) -lSDL2
	gcc -g -Os -DCEU_TIMEMACHINE $(C_FLAGS) main.c -lpthread -lm \
		-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lSDL2_gfx \
		-o $(basename $(CEUFILE)).exe

sim-tst:
	ceu ---timemachine -cpp-args "-I . -DCEU_TIMEMACHINE" sim-tst.ceu
	#gcc main.c $(C_FLAGS) -lSDL2
	gcc -g -Os -DCEU_TIMEMACHINE $(C_FLAGS) main.c -lpthread -lm \
		-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lSDL2_gfx \
		-o sim-tst.exe

demo:
	ceu --cpp-args "-I . -DALL" samples/all.ceu
	#gcc main.c $(C_FLAGS) -lSDL2
	gcc -g -Os main.c -DALL $(C_FLAGS) -lpthread -lm \
		-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lSDL2_gfx \
		-o samples/all.exe

ui-scroll:
	ceu --cpp-args "-D __UI_SCROLL_CEU" ui-scroll.ceu
	gcc -g main.c $(C_FLAGS) -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf -lm

ui-grid:
	ceu --cpp-args "-D __UI_GRID_CEU" ui-grid.ceu
	gcc -g main.c $(C_FLAGS) -lSDL2 -lSDL2_gfx -lm

ui-texture:
	ceu --cpp-args "-D __UI_TEXTURE_CEU" ui-texture.ceu
	gcc main.c $(C_FLAGS) -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf -lm
