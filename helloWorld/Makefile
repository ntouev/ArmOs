CROSS_COMP = ~/x-tools/arm-cortexa9_neon-linux-gnueabihf/bin/arm-cortexa9_neon-linux-gnueabihf
#CROSS_COMP = ~/linaro/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/arm-linux-gnueabihf

CC =  $(CROSS_COMP)-gcc
AS =  $(CROSS_COMP)-as
LD =  $(CROSS_COMP)-ld
OBJCOPY =  $(CROSS_COMP)-objcopy


BINS = kernel.bin

all: $(BINS)

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

kernel.elf: linkscript.ld bootloader.o kernel.o
	$(LD) -T $^ -o $@

bootloader.o: bootloader.s
	$(AS) -g $< -o $@

kernel.o: kernel.c
	$(CC) -c -nostdlib -nostartfiles -lgcc -g -Wall $< -o $@


debug:
	$(CROSS_COMP)-gdb *.elf

disassembly:
	$(CROSS_COMP)-objdump -d *.elf

run:
	sudo qemu-system-arm\
	    -M vexpress-a9\
	    -m 512M\
	    -no-reboot\
	    -nographic\
	    -kernel kernel.bin\
		-monitor telnet:127.0.0.1:22223,server,nowait\
	    #-S -s

clean:
	rm -f *.elf *.o $(BINS)
