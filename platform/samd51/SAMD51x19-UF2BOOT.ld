/*
 * https://github.com/microsoft/uf2-samdx1/blob/master/lib/samd51/gcc/gcc/samd51j20a_flash.ld
 * https://github.com/microsoft/uf2-samdx1/blob/master/lib/samd51/include/samd51j20a.h
 * https://github.com/microsoft/uf2-samdx1/blob/master/lib/samd51/gcc/system_samd51.c
 * https://github.com/microsoft/uf2-samdx1/blob/master/lib/samd51/gcc/gcc/startup_samd51.c
 *
 * https://github.com/adafruit/uf2-samdx1/blob/master/scripts/samd51j19a.ld
 * https://github.com/adafruit/uf2-samdx1/blob/master/boards/pyportal_m4/board_config.h
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

MEMORY
{
    UF2BOOT(r) : ORIGIN = 0x00000000, LENGTH = 16K
    FLASH(rx)  : ORIGIN = 0x00004000, LENGTH = 512K - 16K
    NVRAM(r)   : ORIGIN = 0x00804000, LENGTH = 512
    ESRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 192K
    BSRAM(rwx) : ORIGIN = 0x47000000, LENGTH = 8K
}

STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 32K;
HEAP_SIZE = DEFINED(HEAP_SIZE) ? HEAP_SIZE : DEFINED(__heap_size__) ? __heap_size__ : (LENGTH(ESRAM) - STACK_SIZE);

ENTRY(startup)

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

        . = . + HEAP_SIZE;

        . = ALIGN(4);
        _heap_end = .;
    } >ESRAM

    /* stack */
    . = ALIGN(4);
    _stack_start = .;
    _stack_end = ORIGIN(ESRAM) + LENGTH(ESRAM);
}
