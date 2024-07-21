#!/bin/bash
ls /dev/sdb1
sudo mkdir -p /mnt/pico
sudo mount /dev/sdb1 /mnt/pico
sudo ls /mnt/pico/
sudo cp blink.uf2 /mnt/pico
sudo sync
sudo umount /mnt/pico
sudo rm -rf /mnt/pico
