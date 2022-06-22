#ifndef _TIMER_DELAY_H
#define _TIMER_DELAY_H

#include "sys.h"

/* STM32F407 ------ 使用定时器实现精确延时
测试环境：主频168M  */
void timer_delay_init(void);

//延时nms
//nus为要延时的 ms 数.                                               
void timer_delay_ms(u32 nms);

//延时nus
//nus为要延时的us数.                                               
void timer_delay_us(u32 nus);

#endif
