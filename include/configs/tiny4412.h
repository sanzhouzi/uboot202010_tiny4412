/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2011 Samsung Electronics
 *
 * Configuration settings for the SAMSUNG TINY4412 (EXYNOS4412) board.
 */

#ifndef __CONFIG_TINY4412_H
#define __CONFIG_TINY4412_H

#include <configs/exynos4-common.h>

/* High Level Configuration Options */
#define CONFIG_EXYNOS4412		1	/* which is a EXYNOS4412 SoC */
#define CONFIG_TINY4412			1	/* working with TINY4412*/

/* TINY4412 has 4 bank of DRAM */
#define CONFIG_SYS_SDRAM_BASE		0x40000000
#define PHYS_SDRAM_1			CONFIG_SYS_SDRAM_BASE
#define SDRAM_BANK_SIZE			(256 << 20)	/* 256 MB */

/* memtest works on */
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x3E00000)

#define CONFIG_MACH_TYPE		MACH_TYPE_TINY4412

/* select serial console configuration */

/* Console configuration */
#define CONFIG_DEFAULT_CONSOLE		"console=ttySAC0,115200n8\0"

#define CONFIG_SYS_MEM_TOP_HIDE	(1 << 20)	/* ram console */

#define CONFIG_SYS_MONITOR_BASE	0x00000000

/* Power Down Modes */
#define S5P_CHECK_SLEEP			0x00000BAD
#define S5P_CHECK_DIDLE			0xBAD00000
#define S5P_CHECK_LPA			0xABAD0000

/* MMC SPL */
#define COPY_BL2_FNPTR_ADDR	0x02020030

/* 
	mmcdev			当前启动的MMC设备，启动时根据实际启动的MMC变更为2(sd)或者0(eMMC)
	sddev 			sd卡是MMC2设备
	sdkernelboff 	内核在sd卡上面的偏移(块)
	eMMCkernelboff 	内核在eMMC上面的偏移(块)
	kernelbsize 	内核(块)大小,uboot烧录内核镜像时需要￥改变其值￥
	bootargs		传递给内核的参数


	bootenv
	loadbootenv
	importbootenv
	以上3个环境变量并未使用到，这几个是与在文件系统读取镜像有关的参数，留待以后需要修改时使用。
 */

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x40008000\0" \
	"rdaddr=0x48000000\0" \
	"kerneladdr=0x40008000\0" \
	"ramdiskaddr=0x48000000\0" \
	"console=ttySAC0,115200n8\0" \
	"mmcdev=0\0" \
	"sddev=2\0" \
	"sdkernelboff=0x691\0" \
	"eMMCkernelboff=0x0\0" \
	"kernelbsize=0x2480\0" \
	"bootargs=root=/dev/mmcblk0p1 rootfstype=ext4 console=ttySAC0,115200 init=/linuxrc ctp=2 skipcali=y ethmac=1C:6F:65:34:51:7E\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
        "loadbootscript=load mmc ${mmcdev} ${loadaddr} boot.scr\0" \
        "bootscript=echo Running bootscript from mmc${mmcdev} ...; " \
                "source ${loadaddr}\0"

/* 
 * 参见设备树arch/arm/dts/exynos4412-tiny4412.dts的设置，
 * 设备树bootcmd存在的情况下，这里的配置不使用，这里的配置与文件系统读取镜像有关。
 */
#define CONFIG_BOOTCOMMAND \
	"if mmc rescan; then " \
		"echo SD/MMC found on device ${mmcdev};" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv};" \
			"run importbootenv;" \
		"fi;" \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...;" \
			"run uenvcmd;" \
		"fi;" \
		"if run loadbootscript; then " \
			"run bootscript; " \
		"fi; " \
	"fi;" \
	"load mmc ${mmcdev} ${loadaddr} zImage; bootz ${loadaddr} "



#define CONFIG_CLK_1000_400_200

/* MIU (Memory Interleaving Unit) */
#define CONFIG_MIU_2BIT_21_7_INTERLEAVED

#define RESERVE_BLOCK_SIZE		(512)
#define BL1_SIZE			(8 << 10) /*8 K reserved for BL1*/
#define BL2_SIZE			(16 << 10) /*16 K reserved for BL2*/
#define TINY4412_ENV_SIZE			(16 << 10) /*16 K reserved for ENV*/

#define CONFIG_SPL_MAX_FOOTPRINT	(14 * 1024)

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_INIT_SP_ADDR		0x02040000
#else
//#define CONFIG_SYS_INIT_SP_ADDR		((CONFIG_SYS_LOAD_ADDR) - 0x1000000)
#define CONFIG_SYS_INIT_SP_ADDR		0x42E00000
#endif

/* 
 * SD卡分区:
 *name：	RESERVE--------BL1--------BL2--------UBOOT-------ENV---------KERNEL
 *size：	512B-----------8KB--------16KB-------800KB-------16KB--------10MB
 *
 * eMMC分区:
 * 	boot1分区	mmc dev 0 1：
 *	name：	BL1--------BL2--------UBOOT--------ENV
 *	size：	8KB--------16KB-------800KB--------16KB
 *
 *	user分区	mmc dev 0 0：
 *	name:kernel
 *	size:10MB
 */

/* U-Boot copy size from boot Media to DRAM.*/
#define COPY_UBOOT_SIZE		(800 << 10) /*800 K reserved for UBOOT*/
#define SD_UBOOT_START_OFFSET	((RESERVE_BLOCK_SIZE + BL1_SIZE + BL2_SIZE) / 512)
#define UBOOT_SIZE_BLOC_COUNT	(COPY_UBOOT_SIZE / 512)

/* BL2 copy size from boot Media to DRAM.*/
#define COPY_BL2_SIZE		0x4000
#define BL2_START_OFFSET	((RESERVE_BLOCK_SIZE + BL1_SIZE) /512)
#define BL2_SIZE_BLOC_COUNT	(COPY_BL2_SIZE /512)

#endif	/* __CONFIG_TINY4412_H */
