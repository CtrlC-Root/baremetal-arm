OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

/* https://sourceware.org/binutils/docs/ld/Entry-Point.html */
ENTRY(isr_reset)

SECTIONS
{
    . = ORIGIN(FLASH);

    /* code */
    .text :
    {
        . = ALIGN(4);
        _text_start = .;

        LONG(_stack_end)                /* stack pointer */
        KEEP(*(.vectors .vectors.*))    /* vector table */
        *(.text*)                       /* code */
        *(.glue_7)                      /* GNU LD generated glue for ARM -> Thumb calls */
        *(.glue_7t)                     /* GNU LD generated glue for Thumb -> ARM calls */

        . = ALIGN(4);
        _text_end = .;
    } >FLASH

    /* initialized data, copied to RAM on startup */
    . = ALIGN(4);
    _data_flash_start = .;

    .data :
    {
        . = ALIGN(4);
        _data_ram_start = .;

        *(.data*)

        . = ALIGN(4);
        _data_ram_end = .;
    } >ESRAM AT>FLASH

    /* uninitialized data */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _bss_start = .;

        *(.bss*)
        *(COMMON)        /* common symbols: https://sourceware.org/binutils/docs/ld/Input-Section-Common.html */

        . = ALIGN(4);
        _bss_end = .;
    } >ESRAM

    /* heap */
    .heap (NOLOAD) :
    {
        . = ALIGN(4);
        _heap_start = .;

        /* use all remaining space before the stack */
        . = ORIGIN(ESRAM) + LENGTH(ESRAM) - STACK_SIZE;

        . = ALIGN(4);
        _heap_end = .;
    } >ESRAM AT>ESRAM

    /* stack */
    .stack (NOLOAD) :
    {
        . = ALIGN(4);
        _stack_start = .;

        . = . + STACK_SIZE;

        . = ALIGN(4);
        _stack_end = .;
    } >ESRAM AT>ESRAM
}

HEAP_SIZE = _heap_end - _heap_start;
