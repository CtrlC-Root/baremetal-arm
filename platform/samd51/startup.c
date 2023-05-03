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

extern const void _text_start;
extern const void _text_end;

extern const void _data_flash_start;
extern const void _data_ram_start;
extern const void _data_ram_end;

extern const void _bss_start;
extern const void _bss_end;

// extern const void _stack_start;
// extern const void _stack_end;

/**
 * System Control Block Registers
 */

#define SCB_VTOR_MASK (0x1FFFFFFul << 7U)
static uint32_t* const SCB_VTOR  = (uint32_t*) 0xE000ED08;
// static uint32_t* const SCB_CPACR = (uint32_t*) 0xE000ED88;

/**
 * Application entry point.
 */
extern void main();

/**
 * Default interrupt handler.
 */
void isr_default();

/**
 * Reset interrupt handler.
 */
void isr_reset();

/**
 * Default implementation of standard interrupt handlers.
 *
 * https://en.wikipedia.org/wiki/Weak_symbol
 */

void isr_nmi()              __attribute__ ((weak, alias("isr_default")));
void isr_hard_fault()       __attribute__ ((weak, alias("isr_default")));
void isr_memory_fault()     __attribute__ ((weak, alias("isr_default")));
void isr_bus_fault()        __attribute__ ((weak, alias("isr_default")));
void isr_usage_fault()      __attribute__ ((weak, alias("isr_default")));
void isr_supervisor_call()  __attribute__ ((weak, alias("isr_default")));
void isr_debug_monitor()    __attribute__ ((weak, alias("isr_default")));
void isr_service_call()     __attribute__ ((weak, alias("isr_default")));
void isr_sys_tick()         __attribute__ ((weak, alias("isr_default")));

/**
 * Vector table.
 *
 * Note: The first entry should be a point to the top of the stack but in this
 * project that is automatically added by the linker script.
 *
 * https://interrupt.memfault.com/blog/arm-cortex-m-exceptions-and-nvic
 */
uintptr_t vectors[138] __attribute__((section(".vectors"))) = {
  (uintptr_t) isr_reset,           // Fixed priority -3
  (uintptr_t) isr_nmi,             // Fixed priority -2
  (uintptr_t) isr_hard_fault,      // Fixed priority -1
  (uintptr_t) isr_memory_fault,
  (uintptr_t) isr_bus_fault,
  (uintptr_t) isr_usage_fault,
  (uintptr_t) (0x0ul),             // 7: Reserved
  (uintptr_t) (0x0ul),             // 8: Reserved
  (uintptr_t) (0x0ul),             // 9: Reserved
  (uintptr_t) (0x0ul),             // 10: Reserved
  (uintptr_t) isr_supervisor_call,
  (uintptr_t) isr_debug_monitor,
  (uintptr_t) (0x0ul),             // 13: Reserved
  (uintptr_t) isr_service_call,
  (uintptr_t) isr_sys_tick

  // TODO: define remaining ISRs for SAMD51
};

// Default interrupt handler.
void isr_default() {
  // XXX: infinite loop to create an oppoertunity to attach a debugger?
  while (1);
}

// Reset interrupt handler.
void isr_reset() {
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

  // configure the interrupt vector table base address
  *SCB_VTOR = ((uintptr_t) &_text_start) & SCB_VTOR_MASK;

  // enable the floating point unit
  // *SCB_CPACR |= (0xFu << 20);
  // TODO: __SDB()
  // TODO: __ISB()

  // TODO: initialize C library

  // run the application
  main();

  // XXX: what should we do if the application returns?
  while (1) { }
}
