#CROSS_COMP = ~/x-tools/arm-cortexa9_neon-linux-gnueabihf/bin/arm-cortexa9_neon-linux-gnueabihf
#CROSS_COMP = ~/linaro/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/arm-linux-gnueabihf
CROSS_COMP = arm-none-eabi

CC =  $(CROSS_COMP)-gcc
AS =  $(CROSS_COMP)-as
LD =  $(CROSS_COMP)-gcc #use gcc as a frontend of the linker to avoid missing libs errors
OBJCOPY =  $(CROSS_COMP)-objcopy

BINS = armos.bin
OBJS = bootloader.o init.o shell.o uart_pl011.o

all: $(BINS)

armos.bin: armos.elf
	$(OBJCOPY) -O binary $< $@

armos.elf: linkscript.ld $(OBJS)
	$(LD) -T $^ -o $@ -nostartfiles -lm -lgcc #static linking with libraries

bootloader.o: bootloader.s
	$(AS) -g $< -o $@

init.o: init.c
	$(CC) -c -nostartfiles -g -Wall $< -o $@

shell.o: shell.c
	$(CC) -c -nostartfiles -g -Wall $< -o $@

uart_pl011.o: uart_pl011.c
	$(CC) -c -nostartfiles -g -Wall $< -o $@

debug:
	$(CROSS_COMP)-gdb armos.elf

disassembly:
	$(CROSS_COMP)-objdump -d armos.elf

run:
	sudo qemu-system-arm\
	    -M vexpress-a9\
	    -m 512M\
	    -no-reboot\
	    -nographic\
	    -kernel armos.bin\
		-monitor telnet:127.0.0.1:22223,server,nowait\
	    #-S -s

clean:
	rm -f *.elf *.o $(BINS)
