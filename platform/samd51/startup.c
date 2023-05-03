// Standard Library
#include <stdint.h>
#include <stddef.h> // ptrdiff_t

/**
 * Linker script symbols.
 *
 * These symbols have no size so they are void types but we can still refer
 * to their address in memory which will be filled in from the corresponding
 * linker script symbol.
 */

// extern const void _text_start;
// extern const void _text_end;

extern const void _data_flash_start;
extern const void _data_ram_start;
extern const void _data_ram_end;

extern const void _bss_start;
extern const void _bss_end;

// extern const void _stack_start;
// extern const void _stack_end;

/**
 * Application entry point.
 */
extern void main();

/**
 * Reset vector.
 */
void startup();

/**
 * Vector table.
 *
 * Note: The first entry should be a point to the top of the stack but in this
 * project that is automatically added by the linker script.
 */
uintptr_t vectors[1] __attribute__((section(".vectors"))) = {
  (uintptr_t) startup
  // TODO: leave space for every other vector
};

// Reset vector.
void startup() {
  // copy DATA segment from FLASH to ESRAM
  // https://en.cppreference.com/w/c/types/ptrdiff_t
  // NOTE: linker script aligns DATA segment boundaries to 4 bytes
  ptrdiff_t data_size = &_data_ram_end - &_data_ram_start; // size in bytes
  for (ptrdiff_t offset = 0; offset < data_size; offset += sizeof(uint32_t)) {
    *(((uint32_t*) &_data_ram_start) + offset) = *(((uint32_t*) &_data_flash_start) + offset);
  }

  // initialize BSS segment in ESRAM with zeros
  // https://en.cppreference.com/w/c/types/ptrdiff_t
  // NOTE: linker script aligns BSS segment boundaries to 4 bytes
  ptrdiff_t bss_size = &_bss_end - &_bss_start; // size in bytes
  for (ptrdiff_t offset = 0; offset < bss_size; offset += sizeof(uint32_t)) {
    *(((uint32_t*) &_bss_start) + offset) = 0x00;
  }

  // run the application
  main();

  // XXX: what should we do if the application returns?
  while (1) { }
}
