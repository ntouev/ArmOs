# ArmOs
*__THIS IS AN EDUCATIONAL PROJECT. IF YOU WANT TO COLLABORATE PLEASE CONTACT ME.__*

## Purpose
The purpose of this project is to build a **custom bootloader and a custom kernel** for **Cortex-A9** processors. Right now the only version is implemented in QEMU, and not in real hardware, so there are be some simplifications that should be considered, mentioned [here](#qemu).

## Prerequisites
Install __qemu-system-arm__ as an emulating machine with
```
$ sudo apt install qemu-system-arm
```
and a cross-compiler for compiling binaries for arm architectures.

I am using a custom made one with [crosstool-ng](https://github.com/crosstool-ng/crosstool-ng) and the [linaro](https://www.linaro.org/downloads/). You could choose either of them or another cross-compiler of your choice.

## Usage

* Download or clone the repository.

* In order to compile the project run
```
$ make
```

* To run the emulator with the custom bootloader and kernel run
```
$ make run
```
* To **debug** run
```
$ make debug
```
and then inside the gdb run
```
(qemu) target remote localhost:1234
```
You can also access [qemu monitor](https://en.wikibooks.org/wiki/QEMU/Monitor) typing:
```
$ telnet localhost 22223
```

* To disassembly the whole binary file (bootloader+kernel) run
```
$ make disassembly
```

## QEMU
##### Board
As mentioned above in the first stage of the development, ArmOs will be developed in QEMU. The exact program used is **qemu-system-arm** and the board is **vexprexx-a9**.

##### Running command
With **make run** the following code is running.
```
run:
	sudo qemu-system-arm\
	    -M vexpress-a9\        #specify board vexpress-a9
	    -m 512M\               #512 Mbyte of RAM (and ROM, see README.md)
	    -no-reboot\
	    -nographic\            #pass the uart0 in the terminal and close the pop up QEMU
	    -kernel kernel.bin\    #load kernel.bin as a Linux kernel (in 0x60010000)
		-monitor telnet:127.0.0.1:22223,server,nowait\
	    -S -s                  #toggle on and off for debugging and running mode accordingly
```
##### ROM emulation
Vexpress-a9 does not support the use of ROM memory so the trick to emulate such a functionality is to define 512 Mbyte of RAM (see above) and in the __linkscript to define the two sections of memory, one for refering as RAM and one as ROM__.
```linkscript
MEMORY
{
    ROM (rx) : ORIGIN = 0x60010000, LENGTH = 1M
    RAM (rwx): ORIGIN = 0x70000000, LENGTH = 32M
}
```

##### QEMU Vs Real hardware

Qemu, by default, uses a stub code in the beginning of the RAM at 0x60000000 (see Memory Map in [ARM CoreTile Express A9×4  TRF](https://developer.arm.com/docs/dui0448/latest/preface)) as a BIOS to do some initialization and to jump to address 0x60010000 where the kernel of the Os should be. This is implemented because QEMU assumes that there will be a Linux kernel and there is where we put our custom **bootloader+kernel** file, **kernel.bin**. This is the reason that in file **linkscript.ld** we link the virtual ROM in 0x60010000.

This of course is something that in real hardware would not happen. In fact, in real situations, the above linkscript would link the bootloader in the start of the ROM or somewhere else if the system had a BIOS. Inserting the custom kernel as a Linux kernel is something we do for simplification in emulation and should be considered when the implementation is done for a real system.

This is also the reason that ROM starts from 0x60010000 and not from 0x60000000. If done otherwise QEMU would start its own BIOS in 0x60000000 and after some commands it would jump in address 0x60010000 where it would find nothing.

Below we can see in the first bytes of the RAM the stub code.
```
(qemu) xp /10w 0x60000000
0000000060000000: 0xe3a00000 0xe59f1004 0xe59f2004 0xe59ff004
0000000060000010: 0x000008e0 0x60000100 0x60010000 0x00000000
0000000060000020: 0x00000000 0x00000000

```
And this is the memory (ROM) where kernel.bin is mapped:
```
(qemu) xp /100w 0x60010000
0000000060010000: 0xea000006 0xea00002c 0xeafffffe 0xea00002a
0000000060010010: 0xea000029 0xeafffffe 0xeafffffe 0xeafffffe
0000000060010020: 0xe321f011 0xe59f1094 0xe59fd094 0xe30f0efe
0000000060010030: 0xe34f0efe 0xe151000d 0xb4810004 0xbafffffc
0000000060010040: 0xe321f012 0xe59f107c 0xe59fd07c 0xe30f0afa
0000000060010050: 0xe34f0afa 0xe151000d 0xb4810004 0xbafffffc
0000000060010060: 0xe321f013 0xe59f1064 0xe59fd064 0xe30f0bfb
0000000060010070: 0xe34f0bfb 0xe151000d 0xb4810004 0xbafffffc
0000000060010080: 0xe59f0050 0xe59f1050 0xe59f2050 0xe1510002
0000000060010090: 0xb4903004 0xb4813004 0xbafffffb 0xe3a00000
00000000600100a0: 0xe59f103c 0xe59f203c 0xe1510002 0xb4810004
00000000600100b0: 0xbafffffc 0xeb000022 0xeaffffff 0xef0000ff
00000000600100c0: 0x70000008 0x70001008 0x70001008 0x70002008
00000000600100d0: 0x70002008 0x70003008 0x6001017a 0x70000000
00000000600100e0: 0x70000008 0x70000008 0x70000008 0xe52db004
00000000600100f0: 0xe28db000 0xe24dd00c 0xe50b0008 0xea000007
0000000060010100: 0xe51b2008 0xe2823001 0xe50b3008 0xe3003000
0000000060010110: 0xe3473000 0xe5933000 0xe5d22000 0xe5c32000
0000000060010120: 0xe51b3008 0xe5d33000 0xe3530000 0x1afffff3
0000000060010130: 0xe320f000 0xe320f000 0xe28bd000 0xe49db004
0000000060010140: 0xe12fff1e 0xe92d4800 0xe28db004 0xe300015c
0000000060010150: 0xe3460001 0xebffffe4 0xeafffffe 0x6c6c6548
0000000060010160: 0x6f77206f 0x20646c72 0x6d6f7266 0x72616220
0000000060010170: 0x656d2065 0x216c6174 0x9000000a 0x00001000
0000000060010180: 0x00000000 0x00000000 0x00000000 0x00000000
```
