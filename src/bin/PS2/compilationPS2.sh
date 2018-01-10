#!/bin/bash
rm SLUS_466.51.Demo.iso
mv PS2_demo.elf SLUS_466.51
mkisofs -iso-level 1  -o SLUS_466.51.Demo.iso *
pcsx2 SLUS_466.51.Demo.iso



