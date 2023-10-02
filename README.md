# physics-collision-circle
1.使用OLED12864和mpu6050两个硬件模块，使用RT_Thread Nano操作系统，在stm32C8T6单片机上模拟物理碰撞圆

2.修改了startup_stm32f10x_md.s中堆空间的大小 设置为2.5KB大小

3.修改了了RT_thread 内存堆空间为5KB

4.在运行过程中 共使用了，SROM=10.75KB（共20KB），FLASH大小为19.52KB（共64KB），程序大小为19.71KB（共64KB）

