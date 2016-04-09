###############################################################################
# EDIT
###############################################################################

CEU_DIR  ?= $(error set path to "<ceu>" repository)

###############################################################################
# DO NOT EDIT
###############################################################################

_all: samples

ui:
	export CEU_FLAGS_CPP="-D__UI_TEXTURE_CEU"; make all SRC=arch/ui/texture.ceu
	export CEU_FLAGS_CPP="-D__UI_GRID_CEU";    make all SRC=arch/ui/grid.ceu
	export CEU_FLAGS_CPP="-D__UI_SCROLL_CEU";  make all SRC=arch/ui/scroll.ceu

ARCH_DIR ?= arch
include $(CEU_DIR)/Makefile
