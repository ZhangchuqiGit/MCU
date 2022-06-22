#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"
#include "sys.h"
#include "misc.h"

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void longdelay(u8 s); //长延时
void delay(u8 ms);   // 延时子程序
				    
#endif
