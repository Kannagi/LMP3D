#!/bin/bash
export PATH=$PATH:/usr/local/psxsdk/bin
mkdir -p cd_root
elf2exe PS1_demo.elf PS1_demo.exe
cp PS1_demo.exe cd_root
systemcnf PS1_demo.exe > cd_root/system.cnf
mkisofs -o PS1_demo.hsf -V PS1_demo -sysid PLAYSTATION cd_root
mkpsxiso PS1_demo.hsf PS1_demo.bin /usr/local/psxsdk/share/licenses/infoeur.dat
rm -f PS1_demo.hsf
