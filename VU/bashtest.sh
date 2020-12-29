#!/bin/sh
/home/kannagi/Documents/SDK/PS2/ps2dev/dvp/bin/dvp-as Testvu1Triangle.vcm -o vu1Triangle.o
/home/kannagi/Documents/SDK/PS2/ps2dev/ee/bin/ee-objcopy -O binary vu1Triangle.o vu1Triangle.bin
/home/kannagi/Documents/SDK/PS2/ps2dev/ps2sdk/bin/bin2c vu1Triangle.bin PS2_vu1Triangle.c vu1Triangle

rm vu1Triangle.o
rm vu1Triangle.bin
