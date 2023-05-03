#!/bin/bash

set -e

# locate directories
TOP=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
SOURCE="${TOP}/code"
BUILD="${TOP}/build"

# create build directory if it doesn't exist
mkdir -p "$BUILD"

# COMPILE
# https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
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

# LINK objects into ELF binary
# -nostdlib implies -nodefaultlibs -nostartfiles
# -Wl,--print-map to print linker memory map
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb \
    -nostdlib \
    -Wl,--script "${TOP}/platform/samd51/SAMD51x19-UF2BOOT.ld" \
    -o "${BUILD}/main.elf" "${BUILD}/startup.o" "${BUILD}/main.o"

# EXTRACT code from ELF binary
arm-none-eabi-objcopy -O binary \
    "${BUILD}/main.elf" "${BUILD}/main.bin"

# CONVERT code to UF2 format for flashing with UF2 bootloader
# https://github.com/Microsoft/uf2/blob/master/utils/uf2conv.py
UF2CONV="/home/alex/projects/uf2/utils/uf2conv.py"

if which $UF2CONV; then
    TEXT_BASE=$(\
        arm-none-eabi-objdump \
            --section-headers \
            --section='.text' \
            --wide \
            "${BUILD}/main.elf" \
        | tail -n 1 \
        | tr -s ' ' \
        | cut -d' ' -f5)

    UF2_BASE_ADDRESS="0x${TEXT_BASE}"
    /home/alex/projects/uf2/utils/uf2conv.py --convert \
        --base "${UF2_BASE_ADDRESS}" \
        --output "${BUILD}/main.uf2" \
        "${BUILD}/main.bin"
fi

# XXX: manual verification
# arm-none-eabi-objdump -dhtrwC ./build/main.elf | less
