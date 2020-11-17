#
# Copyright (C) 2011 Samsung Electronics Co., Ltd.
#              http://www.samsung.com/
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
####################################

if [ -z $1 ]
then
    echo "usage: ./sd_fusing.sh <SD Reader's device file>"
    exit 0
fi

if [ -b $1 ]
then
    echo "$1 reader is identified."
else
    echo "$1 is NOT identified."
    exit 0
fi

####################################
#<verify device>

BDEV_NAME=`basename $1`
BDEV_SIZE=`cat /sys/block/${BDEV_NAME}/size`

if [ ${BDEV_SIZE} -le 0 ]; then
	echo "Error: NO media found in card reader."
	exit 1
fi

if [ ${BDEV_SIZE} -gt 32000000 ]; then
	echo "Error: Block device size (${BDEV_SIZE}) is too large"
	exit 1
fi

####################################
# check files

E4412_UBOOT=../../u-boot.bin
E4412_UBOOT_SPL=../../spl/u-boot-spl.bin
MKBL2=../mkbl2
KERNEL=zImage

if [ ! -f ${E4412_UBOOT} ]; then
	echo "Error: u-boot.bin NOT found, please build it & try again."
	exit -1
fi

if [ ! -f ${MKBL2} ]; then
	echo "Error: can not find host tool - mkbl2, stop."
	exit -1
fi

#<make bl2>
${MKBL2} ${E4412_UBOOT_SPL} bl2.bin 14336

####################################
# fusing images
block=512
reserve_size=512
bl1_size=$((8*1024)) #8KB
bl2_size=$((16*1024))	#16KB
env_size=$((16*1024))	#16KB
uboot_size=$((800*1024))	#800KB

echo "-----------------size----------------------"
echo block = $block
echo reserve_size = $reserve_size
echo bl1_size = $bl1_size
echo bl2_size = $bl2_size
echo uboot_size = $uboot_size
echo env_size = $env_size

signed_bl1_position=$((reserve_size/block))
bl2_position=$((signed_bl1_position+bl1_size/block))
uboot_position=$((bl2_position+bl2_size/block))
env_position=$((uboot_position+uboot_size/block))
kernel_position=$((env_position+env_size/block))

echo "-----------------offset----------------------"
echo signed_bl1_position = $signed_bl1_position
echo bl2_position = $bl2_position
echo uboot_position = $uboot_position
echo env_position = $env_position
echo kernel_position = $kernel_position

#<kernel fusing>
echo "---------------------------------------"
echo "kernel fusing"
dd iflag=dsync oflag=dsync if=${KERNEL} of=$1 seek=$kernel_position

#<flush to disk>
sync

####################################
#<Message Display>
echo "---------------------------------------"
echo "U-boot image is fused (at `date +%T`) successfully."
echo "Eject SD card and insert it again."

