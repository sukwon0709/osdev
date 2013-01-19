#!/bin/bash

cd src
make
cd ..

./update_image.sh

sudo losetup /dev/loop0 floppy.img
sudo bochs -f bochsrc
sudo losetup -d /dev/loop0
