
#ifndef   __LED_H_
#define   __LED_H_

#include"stm32f10x.h"

#define  LED_OFF   GPIO_Write(GPIOC,0XFFFF)
#define  LED_ON    GPIO_Write(GPIOC,0X0000)

void LED_GPIO_Config(void);

#endif
