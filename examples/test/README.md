examples/basic
================

This is a simple application for trying out basic linking.

Usage
=====

Build, flash and start the application:
```
export BOARD=your_board
make
make flash
```

Trying out
===========
In order to build use the command 
```
sudo make
``` 

In order to build and directly flash use the command  
```
sudo make flash  
```

In order to enter debug mode  
```
sudo make debug 
```

Example output/Errors
==============
Currently the error - Handler HardFaul ( double fault )
```
Open On-Chip Debugger 0.10.0-dev-00001-g70a14db (2015-06-02-23:59)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : only one transport option; autoselect 'swd'
adapter speed: 500 kHz
adapter_nsrst_delay: 100
cortex_m reset_config sysresetreq
Info : CMSIS-DAP: SWD  Supported
Info : CMSIS-DAP: Interface Initialised (SWD)
Info : CMSIS-DAP: FW Version = 01.1A.00FB
Info : SWCLK/TCK = 1 SWDIO/TMS = 1 TDI = 1 TDO = 1 nTRST = 0 nRESET = 1
Info : CMSIS-DAP: Interface ready
Info : clock speed 500 kHz
Info : SWD IDCODE 0x0bc11477
Info : at91samr21g18.cpu: hardware has 4 breakpoints, 2 watchpoints
target state: halted
target halted due to debug-request, current mode: Thread 
xPSR: 0xf0000000 pc: 0xe28db000 msp: 0xe52db004
** Programming Started **
auto erase enabled
Info : SAMD MCU: SAMR21G18A (256KB Flash, 32KB RAM)
wrote 16384 bytes from file bin/samr21-xpro/test.hex in 2.524429s (6.338 KiB/s)
** Programming Finished **
Error: at91samr21g18.cpu -- clearing lockup after double fault
target state: halted
target halted due to debug-request, current mode: Handler HardFault
xPSR: 0xf1000003 pc: 0xfffffffe msp: 0xe52dafe0
Polling target at91samr21g18.cpu failed, trying to reexamine
Info : at91samr21g18.cpu: hardware has 4 breakpoints, 2 watchpoints
```

Linker Script
==============
Here is the simplest one of the many linker scripts tried out by now.
```
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x200; /* 512 byte */

/* Section Definitions */
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x200; /* 512 byte */

/* Section Definitions */
SECTIONS
{
.text : { *(.text) } 
.data : { *(.data) } 
.bss : { *(.bss) }
.stack (NOLOAD):
{
    . = ALIGN(8);
    . = . + STACK_SIZE;
    . = ALIGN(8);
    _estack = .;
}

}
```
