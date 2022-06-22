/***************STM32F103C8T6**********************
 
 * 文件名  ：main.c
 * 描述    ：模板
 * 备注    ：
 * 接口    ：

********************LIGEN*************************/


#include "stm32f10x.h"
#include "led.h"
#include "usart1.h"
#include "delay.h"
#include "i2c.h"
#include "bh1750.h"

//变量定义
u16  lx;

//主函数
int main(void)
{ 
	
  SystemInit();//配置系统时钟为72M	
	LED_GPIO_Config();//led初始化
	USART1_Config();//串口初始化
	Delay_init(72);//Delay初始化 
	I2C_Init();//i2c初始化
	BH1750_Init();//bh1750初始化
	
	
  while (1)
  {
		lx = BH1750_Read();//读取光照强度
		printf("%d \r\n",lx);
		LED_Twinkle();
		Delay_ms(1000);
  }
}



