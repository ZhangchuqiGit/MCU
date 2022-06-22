/************************************
 * �ļ���  ��exti.c
 * ����    �������ⲿ�жϵķ�ʽ���е������  
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ� PB0��Ϊ�ⲿ�ж�����ӿ� 
 * ��汾  ��ST3.0.0 
**********************************************************************************/
#include "exti.h"
#include "usart.h"

void EXTI0_GPIO_Config()//��PBʱ�ӣ�����PB0����Ϊ����
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//ѡ��Pin0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���Ƶ�����50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);   
}

void EXTI0_Config() //3
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_ClearITPendingBit(EXTI_Line0);	//����жϱ�־

    //�ڿ⺯���У����� GPIO ���ж��ߵ�ӳ���ϵ�ĺ��� GPIO_EXTILineConfig()��ʵ�ֵģ�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);//���ж��� EXTI0����PB0����

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//ѡ���ж���·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж����󣬷��¼�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�͵�ƽ����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //�ⲿ�ж�ʹ��
	EXTI_Init(&EXTI_InitStructure);
}

void EXTI0_NVIC_Config() //2
{
	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    //��GPIOB�Ķ˿�ʱ��
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//ѡ���жϷ���           
        
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//ѡ���ж�ͨ��0��Pin������ͨ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ�ж����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ�ж����ȼ�Ϊ0     
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�                               
	NVIC_Init(&NVIC_InitStructure);             
}

//�ܵ��жϳ�ʼ��  A
void EXTI0_Init()
{
	EXTI0_GPIO_Config();//��PBʱ�ӣ�����PB0����Ϊ����
	EXTI0_NVIC_Config();
	EXTI0_Config();
}
//************************************************
//�ܵ��жϳ�ʼ��  B
void EXTIX_Init(void)
{
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

  //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;//ѡ���ж���·	//KEY2
/**/    
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//����Ϊ�ж����󣬷��¼�����
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�͵�ƽ����
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ⲿ�ж�ʹ��
/**/    
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

 //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;//ѡ���ж���·
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

 //GPIOE.4	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;//ѡ���ж���·
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  //GPIOA.0	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;//ѡ���ж���·
/**/EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�ߵ�ƽ����
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

//*****************EXTI_Line0�ж���·
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//ѡ���ж�ͨ��0//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
//*****************EXTI_Line2�ж���·
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//ѡ���ж�ͨ��2//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);
//*****************EXTI_Line3�ж���·
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//ѡ���ж�ͨ��3//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
//*****************EXTI_Line4�ж���·
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//ѡ���ж�ͨ��4//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
}

///**********************************************

//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	Delay_ms(10);//����
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)	 	 //WK_UP����
	{			
        printf(" //�ⲿ�ж�0  �������\r\n");  

	}
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
}
 
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	Delay_ms(10);//����
	if(KEY2==0)	  //����KEY2
	{


	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE2�ϵ��жϱ�־λ  
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	Delay_ms(10);//����
	if(KEY1==0)	 //����KEY1
	{				 


	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
}
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{
	Delay_ms(10);//����
	if(KEY0==0)	 //����KEY0
	{

	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
}
 
