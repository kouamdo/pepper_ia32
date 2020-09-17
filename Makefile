CC		:= gcc
CFLAGS	:= -m32 -g -ffreestanding -fno-PIC -fno-stack-protector -Wstringop-overflow -Werror
LDFLAGS := ld -m elf_i386 --oformat=binary -Tlinker.ld

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= k_main.o
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
else
EXECUTABLE	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
endif

CINCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
CLIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS		:= $(SOURCES:.c=.o)

SOURCES_ASM := $(wildcard $(patsubst %,%/*.asm, $(SOURCEDIRS)))
OBJECTS_ASM := $(SOURCES_ASM:.asm=.o)

all: boot mixt disk
	clear
	qemu-system-i386 -fda disk.img -d cpu_reset -d int file:serial.log

mixt: k_main.o $(OBJECTS) $(OBJECTS_ASM)
	clear
	@echo "$(OBJECTS)"
	@echo "$(OBJECTS_ASM)"
	$(LDFLAGS) k_main.o $(OBJECTS) $(OBJECTS_ASM) -o bin/kernel.bin
	rm $(OBJECTS)
	rm $(OBJECTS_ASM)

%.o : %.c
	@$(CC) -o $@ -c $< $(CFLAGS) $(CINCLUDES)

%.o:%.asm		
	nasm -felf32 $< -o $@

%.o:%.S		
	nasm -felf32 $< -o $@

boot:
	nasm -g -f elf32 -F dwarf -o $(BIN)/boot.o BOOT/bootloader.asm
	ld -melf_i386 -Ttext=0x7c00 -nostdlib --nmagic -o $(BIN)/boot.elf $(BIN)/boot.o
	objcopy -O binary $(BIN)/boot.elf $(BIN)/boot.bin

disk :
	dd if=/dev/zero of=disk.img bs=512 count=2880
	dd if=bin/boot.bin of=disk.img bs=512 conv=notrunc
	dd if=bin/kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

debug_link_file: k_main.o $(OBJECTS) $(OBJECTS_ASM)
	@echo "$(OBJECTS)"
	@echo "$(OBJECTS_ASM)"
	ld -m elf_i386 --oformat=elf32-i386 -Tlinker.ld k_main.o $(OBJECTS) $(OBJECTS_ASM) -o bin/kernel.elf
	rm $(OBJECTS)
	rm $(OBJECTS_ASM)

debug:
	make debug_link_file
	qemu-system-i386 -fda disk.img -S -s &
	gdb bin/kernel.elf  \
        -ex 'target remote localhost:1234' \
        -ex 'layout src' \
        -ex 'layout reg' \
        -ex 'break main' \
-ex 'continue' 

.PHONY: clean
clean:
	-$(RM) $(BIN)/$(EXECUTABLE)
	-$(RM) $(OBJECTS)


run: all
# 	./$(BIN)/$(EXECUTABLE)

# $(BIN)/$(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(CFLAGS) $(CINCLUDES) $(CLIBS) $^ -o $@ $(LIBRARIES)
	# $(LDFLAGS) $(OBJECTS) $(OBJECTS_ASM) -o bin/kernel.bin
