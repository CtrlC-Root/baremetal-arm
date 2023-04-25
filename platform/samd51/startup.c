// Standard Library
#include <stdint.h>


// XXX: https://stackoverflow.com/a/64271049
// extern uint32_t _stack_top;

extern void main();
void startup();


uintptr_t vectors[1] __attribute__((section(".vectors"))) = {
  (uintptr_t) startup
};

void startup() {
  // TODO: copy data from FLASH to ESRAM
  // TODO: initialize BSS with zeros

  // XXX run the program
  main();

  // XXX
  while (1) { }
}
