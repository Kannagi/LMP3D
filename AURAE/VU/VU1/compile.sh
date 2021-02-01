#!/bin/sh
/usr/local/SDK/PS2/dvp/bin/dvp-as vu1Triangle.vcm -o vu1Triangle.o
/usr/local/SDK/PS2/ee/bin/ee-objcopy -O binary vu1Triangle.o vu1Triangle.bin
/home/kannagi/Documents/Projet/LMP3D/bin/bin2c/bin2c vu1Triangle.bin PS2_vu1Triangle.inc vu1Triangle

#rm vu1Triangle.o
#rm vu1Triangle.bin
