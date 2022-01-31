#!/bin/sh

# create an image of root file system and compress it
find . | cpio -o --format=newc > rootfs.img
gzip -c rootfs.img > rootfs.img.gz
cp rootfs.img.gz ../../linux-5.10/
rm rootfs.img rootfs.img.gz 
