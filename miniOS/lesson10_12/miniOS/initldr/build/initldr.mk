
MAKEFLAGS = -s
HEADFILE_PATH = ../include/
KRNLBOOT_PATH = ../ldrkrl/
CCBUILDPATH	= $(KRNLBOOT_PATH)
include krnlbuildcmd.mh
include ldrobjs.mh
include krnlbuildrule.mh

.PHONY : all everything  build_kernel
all: build_kernel 

build_kernel:everything
	
everything : $(INITLDRIMH_OBJS) $(INITLDRKRL_OBJS) $(INITLDRSVE_OBJS)

