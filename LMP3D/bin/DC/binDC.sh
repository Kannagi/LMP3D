#!/bin/bash
cp ../DC_demo.elf DC_demo.elf
/opt/toolchains/dc/sh-elf/bin/sh-elf-objcopy -O binary DC_demo.elf DC_demo.bin
wine /home/kannagi/Documents/SDK/dreamcast/bootmake_11_cdi4dc_02b/Tools/extra/scramble.exe DC_demo.bin 1ST_READ.BIN
mv DC_demo.bin DS_CORE.BIN




