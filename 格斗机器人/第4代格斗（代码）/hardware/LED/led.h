#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include <stm32f10x.h>	   

//LED端口定义
#define LED PCout(13) // PC13	
void LED_Init(void); //端口初始化		 

#endif

















