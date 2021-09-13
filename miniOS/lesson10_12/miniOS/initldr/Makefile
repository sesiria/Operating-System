MAKEFLAGS = -sR
MKDIR = mkdir
RMDIR = rmdir
CP = cp
CD = cd
DD = dd
RM = rm

BUILD_PATH = ./build
MAKE = make
INITLDRMFLGS = -C $(BUILD_PATH) -f initldr.mk
INITLDRLINKMFLGS = -C $(BUILD_PATH) -f ldrlink.mk
SRCFILE = $(BOOTEXCIMG) $(KRNLEXCIMG) $(LDEREXCIMG) $(SHELEXCIMG)
RSRCFILE = $(BOOTEXCIMG) $(KRNLEXCIMG) $(LDEREXCIMG) $(VDIFNAME) $(VMDKFNAME)
.PHONY : build print clean all

build: clean print all

all:
	$(MAKE) $(INITLDRMFLGS)
	$(MAKE) $(INITLDRLINKMFLGS)
	@echo 'Congratulation! Initldr build finished! ^_^'
clean:
	$(CD) $(BUILD_PATH); $(RM) -f *.o *.bin *.i *.krnl *.s *.map *.lib .btoj *.vdi *vmdk *.eki *.elf
	@echo 'Initldr: all construction file cleaned!... ^_^'

print:
	@echo '*********building the system*************'
