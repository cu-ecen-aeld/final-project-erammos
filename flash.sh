#!/bin/bash
fdisk -l
mount /dev/sdc1 external
ls -la external
read -p "Continue?"
dd if=buildroot/output/images/sdcard.img of=/dev/sdc conv=fdatasync bs=1M
