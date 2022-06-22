/*****************************************************************
@ 基于STM32 & MPU6050_DMP的自平衡小车
@ 固件版本: 测试 V2.0
*****************************************************************/
#include "includes.h"


 

 int main(void)
 {
	USART1_Init(115200,0);
	delay_init();  //初始化延时
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //初始化NVIC
     delay_ms(200);  
	MPU6050_Init();  //初始化陀螺仪
     Timer2_Init(499,71);//Tout（溢出时间）=（ARR+1)(PSC+1)/Tclk =500*72/72000000s=500us
	while(1)
	{ 
                   
     }
}
 



