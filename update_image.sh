#!/bin/bash

sudo losetup /dev/loop1 floppy.img
sudo mount /dev/loop1 /mnt
sudo cp src/kernel /mnt/kernel
sudo umount /dev/loop1
sudo losetup -d /dev/loop1