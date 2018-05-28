CEU_DIR  = $(error set absolute path to "<ceu>" repository)
CEU_SRC  ?= samples/sdl-01.ceu
CEU_ARGS ?=

all:
	ceu --pre --pre-args="-I$(CEU_DIR)/include -I./include $(CEU_ARGS)" \
	          --pre-input=$(CEU_SRC)                                    \
	    --ceu --ceu-err-unused=pass --ceu-err-uninitialized=pass        \
	          --ceu-features-dynamic=true \
	          --ceu-features-pool=true \
	          --ceu-features-pause=true \
	          --ceu-features-os=true \
	    --env --env-types=$(CEU_DIR)/env/types.h                        \
	          --env-main=$(CEU_DIR)/env/main.c                          \
	          --env-output=/tmp/$$(basename $(CEU_SRC) .ceu).c          \
	    --cc --cc-args="-lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx" \
	         --cc-output=/tmp/$$(basename $(CEU_SRC) .ceu)
	cd $(dir $(CEU_SRC)) && /tmp/$$(basename $(CEU_SRC) .ceu)

c:
	ceu --cc --cc-args="-lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx" \
	         --cc-input=/tmp/$$(basename $(CEU_SRC) .ceu).c         \
	         --cc-output=/tmp/$$(basename $(CEU_SRC) .ceu);         \
	/tmp/$$(basename $(CEU_SRC) .ceu);

samples:
	for i in samples/*.ceu; do                                              \
	    echo;                                                               \
	    echo "###############################################";             \
	    echo File: "$$i -> /tmp/$$(basename $$i .ceu)";                     \
	    echo "###############################################";             \
	    echo -n "Press <enter> to start...";                                \
	    read _;                                                             \
	    ceu --pre --pre-args="-I$(CEU_DIR)/include -I./include"             \
	              --pre-input=$$i                                           \
	        --ceu --ceu-err-unused=pass --ceu-err-uninitialized=pass        \
	        --env --env-types=$(CEU_DIR)/env/types.h                        \
	              --env-main=$(CEU_DIR)/env/main.c                          \
	        --cc --cc-args="-lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx" \
	             --cc-output=/tmp/$$(basename $$i .ceu);                    \
		cd samples/ && /tmp/$$(basename $$i .ceu) && cd -                   \
	    echo ">>> OK";                                                      \
	    echo;                                                               \
	    echo;                                                               \
	    echo;                                                               \
	    echo;                                                               \
	    echo;                                                               \
	    echo;                                                               \
	done

.PHONY: samples
