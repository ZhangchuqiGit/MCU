#ifndef __GPIO_JTAG_H
#define	__GPIO_JTAG_H

#include "stm32f10x.h"

void JTAG_SWD_GPIO_Config(void);//stm32的PB3 PB4 PA15引脚可以在使用SWD仿真和烧录的情况下释放出来，作为普通IO口使用


#endif 
