MAKEFLAGS = -sR
MKDIR = mkdir
RMDIR = rmdir
CP = cp
CD = cd
DD = dd
RM = rm

ASM		= nasm
CC		= gcc
LD		= ld
OBJCOPY = objcopy

ASMBFLAGS 	= -f elf -w-orphan-labels
CFLAGS    	= -c -Os -std=c99 -m32 -Wall -Wshadow -W -Wconversion -Wno-sign-conversion -fno-stack-protector -fomit-frame-pointer -fno-builtin -fno-common -ffreestanding -Wno-unused-parameter -Wunused-variable
LDFLAGS    	= -s -static -T miniOS.lds -n -Map miniOS.map
OJCYFLAGS	= -S -O binary

MINIOS_OBJS :=
MINIOS_OBJS += entry.o main.o vgastr.o
MINIOS_ELF   = miniOS.elf
MINIOS_BIN   = miniOS.bin

.PHONY : build clean all link bin

all: clean build link bin

clean:
	$(RM) -f *.o *.bin *.elf

build: $(MINIOS_OBJS)

link: $(MINIOS_ELF)
$(MINIOS_ELF) : $(MINIOS_OBJS)
	$(LD) $(LDFLAGS) -o $@ $(MINIOS_OBJS)

bin: $(MINIOS_BIN)
$(MINIOS_BIN): $(MINIOS_ELF)
	$(OBJCOPY) $(OJCYFLAGS) $< $@

%.o : %.asm
	$(ASM) $(ASMBFLAGS) -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<