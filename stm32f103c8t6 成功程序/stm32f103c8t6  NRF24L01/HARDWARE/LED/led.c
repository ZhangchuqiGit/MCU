#include "led.h"

u8 adc_q=0,humi_q=0,Measure_q=0,ds18b20c_q=0,Human_en=0; 

//��ʼ��PD2Ϊ�����.��ʹ�˿�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initsturct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTCʱ��
	
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_11;
	GPIO_Initsturct.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_Init(GPIOB,&GPIO_Initsturct);
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_11);	
}
void Human_infrared_init(void)//�������
{
	GPIO_InitTypeDef GPIO_Initsturct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTCʱ��
	
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_12; 
	GPIO_Initsturct.GPIO_Mode= GPIO_Mode_IPD;// ��������
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_Init(GPIOB,&GPIO_Initsturct);
}
