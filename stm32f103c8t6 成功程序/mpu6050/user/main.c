/*****************************************************************
@ ����STM32 & MPU6050_DMP����ƽ��С��
@ �̼��汾: ���� V2.0
*****************************************************************/
#include "includes.h"


 

 int main(void)
 {
	USART1_Init(115200,0);
	delay_init();  //��ʼ����ʱ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //��ʼ��NVIC
     delay_ms(200);  
	MPU6050_Init();  //��ʼ��������
     Timer2_Init(499,71);//Tout�����ʱ�䣩=��ARR+1)(PSC+1)/Tclk =500*72/72000000s=500us
	while(1)
	{ 
                   
     }
}
 



