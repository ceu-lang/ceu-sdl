C_FLAGS_ += -DCEU_DEBUG #-DCEU_DEBUG_TRAILS
#C_FLAGS = -DSDL_MATH

C_FLAGS_ += -lpthread -lm \
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
