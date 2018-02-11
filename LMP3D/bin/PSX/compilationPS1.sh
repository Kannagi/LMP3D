#!/bin/bash
rm PS1_demo.iso
export PATH=$PATH:/usr/local/psxsdk/bin
elf2exe PS1_demo.elf demo.exe
mkisofs -iso-level 1 -o PS1_demo.iso *


pcsxr -cdfile PS1_demo.iso

