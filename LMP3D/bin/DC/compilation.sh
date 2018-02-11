#!/bin/bash
cp ../DC_demo.elf DC_demo.elf
/opt/toolchains/dc/sh-elf/bin/sh-elf-objcopy -O binary DC_demo.elf DC_demo.bin
wine /home/kannagi/Documents/SDK/dreamcast/bootmake_11_cdi4dc_02b/Tools/extra/scramble.exe DC_demo.bin 1ST_READ.BIN

#for test with SD CARD
mv DC_demo.bin DS_CORE.BIN


cd ..
mkisofs -C 0,11702 -V DC -G DC/IP.BIN -joliet -rock -l -o DC/DC_demo.iso DATA DC/1ST_READ.BIN

cd DC
./cdi4dc DC_demo.iso DC_demo.cdi

reicast DC_demo.cdi


