#!/bin/sh
/usr/local/SDK/PS2/dvp/bin/dvp-as vu0Triangle.vcm -o vu0Triangle.o
/usr/local/SDK/PS2/ee/bin/ee-objcopy -O binary vu0Triangle.o vu0Triangle.bin
/home/kannagi/Documents/Projet/LMP3D/bin/bin2c/bin2c vu0Triangle.bin PS2_vu0Triangle.c vu0Triangle


