# ArmOs
*__THIS IS AN EDUCATIONAL PROJECT__*

## Purpose
A **custom operaring system ArmOs**, consisting of **a bootloader, a kernel and a user space environment** for **32-bit Cortex-A9** processors, using __ARMv7-A__ architecture.

Right now the only version is implemented in QEMU using [Versatile Express](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.subset.boards.express/index.html) platform, and not in real hardware, so there are some simplifications that should be considered, mentioned [here](#qemu).

* [Motherboard](http://infocenter.arm.com/help/topic/com.arm.doc.dui0447j/DUI0447J_v2m_p1_trm.pdf)
* [Daughterboard](https://developer.arm.com/docs/dui0448/latest/preface)

## Prerequisites
Install __qemu-system-arm__ as an emulating machine with
```
$ sudo apt install qemu-system-arm
```
and a cross-compiler for compiling binaries for arm architectures.
The current version uses a custom made one with [crosstool-ng](https://github.com/crosstool-ng/crosstool-ng) and the [linaro](https://www.linaro.org/downloads/) cross-compiler.

## Usage/Development

* Download or clone the repository.

* In order to compile the project run
```
$ make
```

* Run the emulator with
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
Assuming that the VM is powered on, access [qemu monitor](https://en.wikibooks.org/wiki/QEMU/Monitor) typing:
```
$ telnet localhost 22223
```

* To disassembly the whole binary file run
```
$ make disassembly
```

## QEMU

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
Versatile Express series, in QEMU, do not support the use of ROM, so the trick to emulate such a functionality is to define 512 Mbyte of RAM (see above in the *make run* command) and in the __linker script to define the two sections of memory, one refering as RAM and one as ROM__.
```
MEMORY
{
    ROM (rx) : ORIGIN = 0x60010000, LENGTH = 1M
    RAM (rwx): ORIGIN = 0x70000000, LENGTH = 32M
}
```

##### QEMU Vs Real hardware

Qemu, by default, uses a stub code in the beginning of the RAM at 0x60000000 (see Memory Map in [ARM CoreTile Express A9×4  TRF](https://developer.arm.com/docs/dui0448/latest/preface)) as a BIOS to do some initialization and to jump to address 0x60010000 where the kernel of the Os should be.

```
(qemu) xp /10w 0x60000000
0000000060000000: 0xe3a00000 0xe59f1004 0xe59f2004 0xe59ff004
0000000060000010: 0x000008e0 0x60000100 0x60010000 0x00000000
0000000060000020: 0x00000000 0x00000000

```
 This is implemented because adding the binary file with the __-kernel__ flag maps the code in address 0x60010000, since this is the address where a Linux kernel would have been mapped. *(QEMU is developed to emulate the Linux kernel.)*

In file **linkscript.ld**, we link the virtual __ROM in 0x60010000. RAM is linked in 0x70000000__.

*This is, of course, something that in real hardware would not happen. If this project is implemented on a real platform in the future, this should be considered. __The linker script should be modified, in this case.__*

This is the memory (ROM) where kernel.bin is mapped:
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
