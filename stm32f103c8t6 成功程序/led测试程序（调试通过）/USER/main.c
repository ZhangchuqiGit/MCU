/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    ：电机测试的程序整合，包含电压、电流、转速和油门的采集
             以及将油门转化为电机控制信号pwm波
 * 实验平台：STM32F103C8T6
 * 备注    ：系统整合1.0版本，程序目前可以实现三路独立的AD转换，定时串口发送数据，转速以外部中断方式计数;
 * 接口    ：AD0-AD2对应PA0-PA2; 转速接口PB0;PWM输出接口待定

********************LIGEN*************************/

#include "stm32f10x.h"
#include "led.h"


int main(void)
{ 
	
  SystemInit();//配置系统时钟为72M	
	LED_GPIO_Config();//led初始化
	
	
  while (1)
  {
		LED_Twinkle();
		Delayms(10);
  }
}



