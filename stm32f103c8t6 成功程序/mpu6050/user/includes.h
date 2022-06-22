#ifndef _includes_H
#define _includes_H

//#include "includes.h"


#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "wdg.h"
#include "led.h"


#include "mpu6050.h"
#include "iic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

//公用函数声明
float map(float input,float input_min,float input_max,float output_min,float output_max);

#endif 
