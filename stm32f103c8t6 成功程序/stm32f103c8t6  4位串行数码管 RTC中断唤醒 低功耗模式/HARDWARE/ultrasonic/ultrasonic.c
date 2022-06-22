/*************************************
 * �ļ���  ��main.c
 * ����    �����������
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��2017.01.08
 * �ӿ�    ��

**********************************************************************************/
#include "ultrasonic.h"

u16 Distance=0; //������ľ���//ǰ�����
u16 Distance_B=0; //������ľ���//�󷽲��

/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void Ultrasonic_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

/*   ǰ�����    */
	GPIO_InitStructure.GPIO_Pin = Trig;					 //PA1�� TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOA, &GPIO_InitStructure);	                 //��ʼ������GPIO 

	GPIO_InitStructure.GPIO_Pin = Echo;				     //PA0�� ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //��Ϊ��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);						 //��ʼ��GPIO
	
/*   �󷽲��    */
	GPIO_InitStructure.GPIO_Pin = Trig_B;					 //PA1�� TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOA, &GPIO_InitStructure);	                 //��ʼ������GPIO 

	GPIO_InitStructure.GPIO_Pin = Echo_B;				     //PA0�� ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //��Ϊ��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);						 //��ʼ��GPIO
	
//    TIM_Cmd(TIM2, DISABLE);	////��ʱ��2ʧ��
/* �ж�/��� ʱ�� TIM2 Tout us= ( 1000 * 72 )/ 72 = 1 000 us  */
//	TIM2_Int_Init(10000-1,72-1); 
/* "TIM_GetCounter(TIM2)"->ʱ��������� ?us/10000.0=?ms * "340"->�����ٶ�340m/s= 340mm/ms / "2.0"->����·��;  */
}
#if 0
//----------------------- TIM2 ---------------------------------
void TIM2_Int_Init(u16 arr,u16 psc)
{
//��ʱ��TIM2��ʼ��   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//�ж����ȼ�NVIC����
    NVIC_InitTypeDef NVIC_InitStructure; 
    
//��ʱ��TIM2��ʼ��   **************************************************  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);//ʱ��ʹ��
    
    TIM_DeInit(TIM2);//��λ��ʱ��2���мĴ���
    
    TIM_TimeBaseStructure.TIM_Period=arr;	//�Զ���װ�ؼĴ������ڵ�ֵ
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	//ʱ��Ԥ��Ƶ�� 72M/72      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
/*λ9:8 CKD[1:0]: ʱ�ӷ�Ƶ���Ӷ����ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)ʹ�õĲ���Ƶ��֮��ķ�Ƶ������*/
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
   
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// �������жϱ�־ 
   
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//ʹ��ָ����TIM2�ж�,��������ж�
	TIM_ITConfig(TIM2,TIM_IT_Trigger,ENABLE);//��������жϣ�������ʽ�ж�

//�ж����ȼ�NVIC����   **************************************************     
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //����NVIC�жϷ���0:4λ��ռ���ȼ���4λ��Ӧ���ȼ� 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	   //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��   
    NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
 
//    TIM_Cmd(TIM2, ENABLE);	// ����ʱ��    
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//�ȹرյȴ�ʹ��      
    /*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//�ȹر�  //ʹ������  ʹ��TIME2��ʱ��
    TIM_Cmd(TIM2, ENABLE);	// ����ʱ��      
*/
}
#endif

/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void Ultrasonic_Measure(void)
{
	GPIO_SetBits(GPIOA,Trig);//��>10US�ĸߵ�ƽ
	delay_us(20);//��ʱ20US
	GPIO_ResetBits(GPIOA,Trig);
	while(!GPIO_ReadInputDataBit(GPIOA,Echo));//�ȴ��ߵ�ƽ
	TIM_Cmd(TIM2, ENABLE);//����ʱ��
	while(GPIO_ReadInputDataBit(GPIOA,Echo));//�ȴ��͵�ƽ
	TIM_Cmd(TIM2, DISABLE);//��ʱ��2ʧ��
	
//	TIM2_Int_Init(10000-1,720-1);// �ж�/��� ʱ�� Tout us= ( 10000==�������� * 72 )/ 72 = 100 000 uS
/* "TIM_GetCounter(TIM2)"->ʱ��������� ?us/10000=?ms * "340"->�����ٶ�340m/s= 340mm/ms / "2.0"->����·��;  */
	Distance=TIM_GetCounter(TIM2)*340/200;
	TIM_SetCounter(TIM2,0);
}
void Ultrasonic_Measure_B(void)//�󷽲��
{
	GPIO_SetBits(GPIOA,Trig_B);//��>10US�ĸߵ�ƽ
	delay_us(20);//��ʱ20US
	GPIO_ResetBits(GPIOA,Trig_B);
	while(!GPIO_ReadInputDataBit(GPIOA,Echo_B));//�ȴ��ߵ�ƽ
	TIM_Cmd(TIM2, ENABLE);//����ʱ��
	while(GPIO_ReadInputDataBit(GPIOA,Echo_B));//�ȴ��͵�ƽ
	TIM_Cmd(TIM2, DISABLE);//��ʱ��2ʧ��
	
//	TIM2_Int_Init(10000-1,720-1);// �ж�/��� ʱ�� Tout us= ( 10000==�������� * 72 )/ 72 = 100 000 uS
/* "TIM_GetCounter(TIM2)"->ʱ��������� ?us/10000=?ms * "340"->�����ٶ�340m/s= 340mm/ms / "2.0"->����·��;  */
	Distance_B=TIM_GetCounter(TIM2)*340/200;
	TIM_SetCounter(TIM2,0);
}
