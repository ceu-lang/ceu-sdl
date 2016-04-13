###############################################################################
# EDIT
###############################################################################

CEU_DIR ?= $(error set absolute path to "<ceu>" repository)

###############################################################################
# DO NOT EDIT
###############################################################################

SDL_DIR ?= .
ARCH_DIR ?= arch
include $(CEU_DIR)/Makefile

ifneq ($(MAKECMDGOALS),link)
ifeq ("$(wildcard $(SDL_DIR)/arch/up)","")
$(error run "make link")
endif
endif

link:
	rm -f arch/up
	ln -s `readlink -f $(CEU_DIR)/arch` $(SDL_DIR)/arch/up

ui-test:
	make all CEU_FLAGS_CPP="-D__UI_TEXTURE_CEU" SRC=arch/ui/texture.ceu
	make all CEU_FLAGS_CPP="-D__UI_GRID_CEU"    SRC=arch/ui/grid.ceu
	make all CEU_FLAGS_CPP="-D__UI_SCROLL_CEU"  SRC=arch/ui/scroll.ceu
