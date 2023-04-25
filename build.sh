#!/bin/bash

# https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
# https://github.com/Microsoft/uf2/blob/master/utils/uf2conv.py

TOP=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

SOURCE="${TOP}/code"
BUILD="${TOP}/build"

mkdir -p "$BUILD"

# COMPILE
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb \
    -ffreestanding \
    -Wall -Wextra \
    -Og -g \
    -c -o "${BUILD}/startup.o" "${TOP}/platform/samd51/startup.c"

arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb \
    -ffreestanding \
    -Wall -Wextra \
    -Og -g \
    -c -o "${BUILD}/main.o" "${SOURCE}/main.c"

# XXX
#arm-none-eabi-objdump -dhtrwC ./build/main.o

# LINK
# -nostdlib implies -nodefaultlibs -nostartfiles
# -Wl,--print-map to print linker memory map
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb \
    -nostdlib \
    -Wl,--script "${TOP}/platform/samd51/SAMD51x19-UF2BOOT.ld" \
    -o "${BUILD}/main.elf" "${BUILD}/startup.o" "${BUILD}/main.o"

# arm-none-eabi-objdump -dhtrwC ./build/main.elf

# XXX
arm-none-eabi-objcopy -O binary \
    "${BUILD}/main.elf" "${BUILD}/main.bin"

# XXX
#/home/alex/projects/uf2/utils/uf2conv.py --base 0x4000 --convert \
#    --output "${BUILD}/main.uf2" "${BUILD}/main.bin"
