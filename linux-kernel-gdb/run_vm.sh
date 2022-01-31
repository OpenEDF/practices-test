#!/bin/bash

# Boot the emulated system to a shell prompt.

QUEM=qemu-system-x86_64

run_emulator()
{
  [ ! -z "$DEBUG" ] && set -x
  $QUEM -nographic \
        -kernel arch/x86_64/boot/bzImage \
        -initrd rootfs.img.gz \
        -append "console=ttyS0,115200 nokaslr root=/dev/ram rdinit=/sbin/init" \
        -gdb tcp::2222 \
        -S 
}

if [ "$1" != "--norun" ]
then
  run_emulator
fi
