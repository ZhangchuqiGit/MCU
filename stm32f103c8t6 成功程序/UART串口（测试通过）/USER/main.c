/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    ��������ӡ��UART test��,ͨ�����ڵ������������ӷ������ݣ����ӽ��յ����ݺ������ش������ԡ�         
 * ʵ��ƽ̨��STM32F103C8T6
 * ��汾  ��ST3.0.0  																										  

********************LIGEN*************************/

#include "stm32f10x.h"
#include "usart1.h"



int main(void)
{  
	SystemInit();	//����ϵͳʱ��Ϊ 72M 
   
	USART1_Config(); //USART1 ���� 
	
	printf("UART test \r\n");

  while (1)
  {	 
    UART1Test();
  }
}




