#ifndef _TIMER_DELAY_H
#define _TIMER_DELAY_H

#include "sys.h"

/* STM32F407 ------ ʹ�ö�ʱ��ʵ�־�ȷ��ʱ
���Ի�������Ƶ168M  */
void timer_delay_init(void);

//��ʱnms
//nusΪҪ��ʱ�� ms ��.                                               
void timer_delay_ms(u32 nms);

//��ʱnus
//nusΪҪ��ʱ��us��.                                               
void timer_delay_us(u32 nus);

#endif
