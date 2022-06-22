/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    ：开机打印“UART test”,通过串口调试软件，向板子发送数据，板子接收到数据后，立即回传给电脑。         
 * 实验平台：STM32F103C8T6
 * 库版本  ：ST3.0.0  																										  

********************LIGEN*************************/

#include "stm32f10x.h"
#include "usart1.h"



int main(void)
{  
	SystemInit();	//配置系统时钟为 72M 
   
	USART1_Config(); //USART1 配置 
	
	printf("UART test \r\n");

  while (1)
  {	 
    UART1Test();
  }
}




