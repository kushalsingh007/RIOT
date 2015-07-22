examples/basic
================

This is a simple application for trying out basic linking (involving an led).

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

Example output
==============
The LED will be blink for a short duration (LED_RED_ON - wait - LED_RED_OFF).  

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
