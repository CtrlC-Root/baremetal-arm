// Standard Library
#include <stdint.h>

#define PORT_BASE 0x41008000
#define PORT(PI) (PORT_BASE + (0x80 * PI))
#define PORT_DIRSET(PI) (PORT(PI) + 0x08)
#define PORT_OUTCLR(PI) (PORT(PI) + 0x14)
#define PORT_OUTSET(PI) (PORT(PI) + 0x18)


// page 804, 808
// static uint32_t* const PORT_BASE = (uint32_t*) (0x41008000);
// static uint32_t* const PORT_A    = PORT_BASE + (0x80 * 0);
// static uint32_t* const PORT_B    = PORT_BASE + (0x80 * 1);


// static uint32_t* const PORT_A_DIR       = PORT_A + 0x00;
// static uint32_t* const PORT_A_DIRCLR    = PORT_A + 0x04;
static uint32_t* const PORT_A_DIRSET    = (uint32_t*) PORT_DIRSET(0);
// static uint32_t* const PORT_A_DIRTGL    = PORT_A + 0x0C;
// static uint32_t* const PORT_A_OUT       = PORT_A + 0x10;
static uint32_t* const PORT_A_OUTCLR    = (uint32_t*) PORT_OUTCLR(0);
static uint32_t* const PORT_A_OUTSET    = (uint32_t*) PORT_OUTSET(0);
// static uint32_t* const PORT_A_OUTTGL    = PORT_A + 0x1C;
// static uint32_t* const PORT_A_IN        = PORT_A + 0x20;
// static uint32_t* const PORT_A_CTRL      = PORT_A + 0x24;
// static uint32_t* const PORT_A_WRCONFIG  = PORT_A + 0x28;
// static uint32_t* const PORT_A_EVCTRL    = PORT_A + 0x2C;
// static uint32_t* const PORT_A_PMUX0     = PORT_A + 0x30 + 0x0;
// ...
// static uint32_t* const PORT_A_PMUX15    = PORT_A + 0x30 + 0xF;
// static uint32_t* const PORT_A_PINCFG0   = PORT_A + 0x40 + 0x00;
// ...
// static uint32_t* const PORT_A_PINCFG31  = PORT_A + 0x40 + 0x1F;

// static uint32_t* const PORT_B_DIR       = PORT_B + 0x00;
// static uint32_t* const PORT_B_DIRCLR    = PORT_B + 0x04;
// static uint32_t* const PORT_B_DIRSET    = PORT_B + 0x08;
// static uint32_t* const PORT_B_DIRTGL    = PORT_B + 0x0C;
// static uint32_t* const PORT_B_OUT       = PORT_B + 0x10;
// static uint32_t* const PORT_B_OUTCLR    = PORT_B + 0x14;
// static uint32_t* const PORT_B_OUTSET    = PORT_B + 0x18;
// static uint32_t* const PORT_B_OUTTGL    = PORT_B + 0x1C;
// static uint32_t* const PORT_B_IN        = PORT_B + 0x20;
// static uint32_t* const PORT_B_CTRL      = PORT_B + 0x24;
// static uint32_t* const PORT_B_WRCONFIG  = PORT_B + 0x28;
// static uint32_t* const PORT_B_EVCTRL    = PORT_B + 0x2C;
// static uint32_t* const PORT_B_PMUX0     = PORT_B + 0x30 + 0x0;
// ...
// static uint32_t* const PORT_B_PMUX15    = PORT_B + 0x30 + 0xF;
// static uint32_t* const PORT_B_PINCFG0   = PORT_B + 0x40 + 0x00;
// ...
// static uint32_t* const PORT_B_PINCFG31  = PORT_B + 0x40 + 0x1F;

static uint32_t pattern = 0xABCD0123;

void main() {
    volatile uint32_t i;

    // configure the LED pin (GPIO 13, PA16)
    *PORT_A_DIRSET = (1u << 16);    // configure pin as output

    // blink the LED
    while (1) {
        // rotate the pattern 1 bit left
        pattern = (pattern << 1) | (pattern >> 31);

        // turn on LED
        *PORT_A_OUTSET = ((pattern & 0x00000001) << 16);

        // sleep
        for (i = 0; i < 0xFFFF; i++) {
            __asm("nop");
        }

        // turn off LED
        *PORT_A_OUTCLR = (1u << 16);

        // sleep
        for (i = 0; i < 0xFFFF; i++) {
            __asm("nop");
        }
    }
}
