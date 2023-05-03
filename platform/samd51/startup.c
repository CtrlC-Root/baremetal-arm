// Standard Library
#include <stdint.h>

/**
 * Linker script memory pointers.
 *
 * https://stackoverflow.com/a/64271049
 */
extern uint8_t* _text_start;
extern uint8_t* _text_end;
extern uint8_t* _data_start;
extern uint8_t* _data_end;
extern uint8_t* _bss_start;
extern uint8_t* _bss_end;
extern uint8_t* _stack_start;
extern uint8_t* _stack_end;

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
};

// Reset vector.
void startup() {
  // TODO: copy data from FLASH to ESRAM

  // initialize BSS region with zeros
  for (uint8_t* memory = _bss_start; memory < _bss_end; memory++) {
    (*memory) = 0;
  }

  // run the application
  main();

  // XXX: what should we do after the application returns?
  while (1) { }
}
