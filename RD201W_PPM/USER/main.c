
#include <stm32f10x.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "math.h"
#include "stdlib.h"
#include "timer.h"
#include "GPIO_JTAG.h"
#include "led.h"


int main(void)
{
	u16 num=500;
	u8 flag=0;
	SystemInit();//ϵͳʱ������,����ϵͳʱ��Ϊ72M	
	delay_init(72);//��ʱ��ʼ��  
	JTAG_SWD_GPIO_Config();//��ʹ��JTAG���ԣ���Ӧ��IO����PB3,PB4,PA15������Ϊ��ͨIO��ʹ�ã�������ʹ��SWD�������¼��������ͷų���
	/*  �ж����ȼ�NVIC����  */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	uart1_init(115200);//USART1_TX PA.9 ; USART1_RX PA.10 
	LED_Init();
	
/* void TIM3_Int_Init(u16 arr,u16 psc)
   TIM3_Int_Init(5000-1,7200-1);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ50ms 
   �ж�/���  ʱ�� t = ( 72M /(psc+1) ) * ( arr+1 ) = 72M * arr / psc
				   t = ( 72M / 7200 ) * ( 5000 ) = 72M * 5000 / 7200 = 50ms
				   t = ( 72M / 72 ) * ( 20000 ) = 72M * 20 000 / 72 = 20ms  */
//	TIM4_PWM_SG90_MG90_Init();//PB6-9 : TIM4 CH1-4
/* ��ʱ��2 ͨ��2 (PA1) ���� PPM �������� */
	TIM2_Capture_PPM_Init();	//��1Mhz��Ƶ�ʼ��� 
	while(1)
	{
		LED=1;
//		//����ڶ�
//		SG90_MG90_1(num);//500-2500
//		//����ڶ�
//		SG90_MG90_2(num);//500-2500
//		//����ڶ�
//		SG90_MG90_3(num);//500-2500
//		//����ڶ�
//		SG90_MG90_4(num);//500-2500
		delay_ms(200);
		LED=0;
		delay_ms(200);
		printf("zcq : %d\t",num);
		if(flag==0)
		{
			num+=20;
			if(num>=2500)
			{
				flag=1;
				printf("\r\n");
			}
		}
		else
		{
			num-=20;
			if(num<=500)
			{
				flag=0;	
				printf("\r\n");
			}
		}
		if(ppm_rx[0])//�ɹ�������һ��������
		{
			printf("����:%d ǰ��:%d ����:%d ����:%d chanel5:%d chanel6:%d\r\n",
				ppm_rx[1],ppm_rx[2],ppm_rx[3],ppm_rx[4],ppm_rx[5],ppm_rx[6]);
			ppm_rx[0]=0;
		}
	}
}
