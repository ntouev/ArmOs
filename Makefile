CC =  ~/x-tools/arm-cortexa9_neon-linux-gnueabihf/bin/arm-cortexa9_neon-linux-gnueabihf-gcc
AS =  ~/x-tools/arm-cortexa9_neon-linux-gnueabihf/bin/arm-cortexa9_neon-linux-gnueabihf-as
LD =  ~/x-tools/arm-cortexa9_neon-linux-gnueabihf/bin/arm-cortexa9_neon-linux-gnueabihf-ld
OBJCOPY =  ~/x-tools/arm-cortexa9_neon-linux-gnueabihf/bin/arm-cortexa9_neon-linux-gnueabihf-objcopy

CFLAGS = -Wall
#CFLAGS += -static

BINS = test.bin

all: $(BINS)

test.bin: test.elf
	$(OBJCOPY) -O binary $< $@

test.elf: test.ld test.o bootloader.o
	$(LD) -T $^ -o $@

test.o: test.c
	$(CC) $(CFLAGS) $< -o $@

bootloader.o: bootloader.s
	$(AS) $< -o $@

clean:
	rm -f *.elf *.o $(BINS)
