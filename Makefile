CEU_DIR = $(error set absolute path to "<ceu>" repository)

samples:
	for i in samples/sdl-*.ceu; do                                          \
		echo;                                                               \
		echo "#####################################";                       \
		echo File: "$$i";                                                   \
		echo "#####################################";                       \
		echo -n "Press <enter> to start...";                                \
		read _;                                                             \
	    ceu --pre --pre-args="-I$(CEU_DIR)/include -I./include"             \
	              --pre-input=$$i                                           \
	        --ceu                                                           \
	        --env --env-header=$(CEU_DIR)/env/header.h                      \
	              --env-main=$(CEU_DIR)/env/main.c                          \
	        --cc --cc-args="-lm -llua5.3 -lpthread -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx" \
	             --cc-output=/tmp/ceu.exe;                                  \
		/tmp/ceu.exe;                                                       \
		echo ">>> OK";                                                      \
		echo;                                                               \
		echo;                                                               \
		echo;                                                               \
		echo;                                                               \
		echo;                                                               \
		echo;                                                               \
	done

.PHONY: samples
