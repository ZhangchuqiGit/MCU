
//**************************************
/***************STM32F103C8T6**********************
 * �ļ���  ��led.c
 * ����    : GPIO_Pin_8����
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��ʵ�ְ���GPIO_Pin_8����������˸
 * �ӿ�    ��PB12

********************LIGEN*************************/

#include "led.h"


 /***************  ����I/O�� *******************/
void LED_GPIO_Config(void)	
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PC�˿�ʱ��  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PC�˿�
    
//    GPIO_SetBits(GPIOB, GPIO_Pin_8);//�����	 // GPIO_ResetBits
//    GPIO_SetBits(GPIOB, GPIO_Pin_9);//���1	 // GPIO_ResetBits
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);//���0
    GPIO_ResetBits(GPIOB, GPIO_Pin_9);//���0
    //  GPIO_SetBits���ԶԶ��IO��ͬʱ������λ1 

 /*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
    GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
    GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 
*/
    
}


void LED_Toggle(void)//led��ת��ƽ
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8))));//led��ƽ��ת
}
void LED_On()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8,(BitAction)0);//GPIO_Pin_8 = GPIO_Pin_8
}
void LED_Off()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8,(BitAction)1);//GPIO_WriteBit�Ƕ�һ��IO�ڽ���д������������д0����д1
}

void Delayus(u16 i)
{
	unsigned char t = 0;
	for(;i>0;i--)
	{
		for(t=0;t<2;t++)
		{}
	}
}
void Delayms(u16 t)
{
	while(t--)
	{
		Delayus(1000);
	}
}
void LED_Twinkle()//led��һ��
{
	LED_On();
	Delayms(10);
	LED_Off();
}
