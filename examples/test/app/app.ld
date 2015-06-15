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
