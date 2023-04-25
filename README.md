# dgol

Distributed game of life.

## Development

Install system requirements:

* arm-none-eabi-binutils
* arm-none-eabi-gcc
* arm-none-eabi-gdb
* arm-none-eabi-newlib

On Arch Linux:

```bash
pacman -S arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-newlib
```

On Void Linux:

```bash
xbps-install cross-arm-none-eabi cross-arm-none-eabi-newlib
```

Build the project:

```bash
./build.sh
```

## References

* [UF2](https://github.com/Microsoft/uf2)
  * https://makecode.com/blog/one-chip-to-flash-them-all
  * https://learn.adafruit.com/adafruit-metro-m0-express/uf2-bootloader-details
  * https://github.com/microsoft/uf2/blob/master/utils/uf2conv.py
* SAMD51
  * https://github.com/microsoft/uf2-samdx1
  * https://github.com/adafruit/uf2-samdx1
  * https://learn.adafruit.com/how-to-program-samd-bootloaders
* [Adafruit PyGamer](https://www.adafruit.com/product/4242)
  * https://learn.adafruit.com/adafruit-pygamer
  * https://github.com/adafruit/Adafruit_Arcada
* [Adafruit PyPortal](https://www.adafruit.com/product/4116)
  * https://learn.adafruit.com/adafruit-pyportal
* Bare Metal
  * https://gitlab.cba.mit.edu/pub/hello-world/atsamd51/-/tree/master/
  * https://arobenko.github.io/bare_metal_cpp/
  * https://interrupt.memfault.com/tag/zero-to-main/
  * https://castlerock.se/2019/10/forth-for-cortex-m4-part-i-blinkenlights/
  * https://microchipdeveloper.com/32arm:sam-bare-metal-c-programming
  * http://cs107e.github.io/guides/
  * https://github.com/shawn-dsilva/tm4c-linux-template
* LD
  * https://sourceware.org/binutils/docs-2.40/ld.html
* Newlib
  * https://sourceware.org/newlib/libc.html
