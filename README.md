# uboot2020_10_tiny4412
代码适合的开发板：
CPU处理器：Samsung ARM Cortex-A9 四核Exynos 4412 Quad-core处理器
DDR3 RAM内存： 1G
FLASH存储：4G eMMC

编译方法：
如果你的编译器跟我的一样，可以直接执行如下命令编译：
make tiny4412_defconfig & make

如果编译器不一样，请修改Makefile,指定你自己的编译器，然后再执行上面的命令。
我使用的编译器见博客：https://blog.csdn.net/lgc1990/article/details/109154007

分支介绍：
main——初步移植成功的uboot代码，并不能实际使用，还需继续完善。对应博客：https://blog.csdn.net/lgc1990/article/details/109160426
