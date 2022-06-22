/***************STM32F103C8T6**********************
 * �ļ���  ��timer.c
 * ����    : TIMx��ʱ��
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��ʹ�ö�ʱ��������pwm�������ƶ������
 * �ӿ�    ��TIM1, CH1-PB13, CH2-PB14, //CH3-PB15
****************STM32F103C8T6**********************/
#include "timer.h"
#include "misc.h"
#include "usart.h"

/***************
�ж�/���  ʱ�� t=  (  72M /(TIM_Prescaler+1)  )  *  (  TIM_Period+1  )
//TIM2_Int_Init(1000-1,72-1);
1000-1    72-1  = 1ms 
�ж�����Ϊ1ms
//  TIM3_Int_Init(5000-1,7200-1);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
// �ж�/���  ʱ�� t=  (  72M /(psc+1)  )  *  (  arr+1  )
****************/

//-----------------------------------------------------------------
//----------------------------------------------------------------
/*����TIM1�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ� */
void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
	u16 CCR1_Val = 0;        
	u16 CCR2_Val = 0;
	//u16 CCR3_Val = 50000;


/* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;//��ʱ������,20ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;//ʱ��Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//������ʽ�����ϼ���

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

/* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//ѡ��ʱ��ģʽ��tim�������2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//����
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

/* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	

/* PWM1 Mode configuration: Channel3 */
/*
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	*/

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

void TIM1_Int_Init()/* ����TIM1�������PWMʱ�õ���I/O  */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOB, ENABLE);//��ʱ��1ʱ��ʹ��//GPIOBʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 ;//| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*���úͳ�ʼ��TIM1 */
void TIM1_PWM_Init(void)
{
	TIM1_Int_Init();
	TIM1_Mode_Config();	
}

//-----------------------------------------------------------------
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

//�ж����ȼ�NVIC����   **************************************************     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //����NVIC�жϷ���0:4λ��ռ���ȼ���4λ��Ӧ���ȼ� 
  
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	   //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��   
    NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
 
    TIM_Cmd(TIM2, ENABLE);	// ����ʱ��    
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//�ȹرյȴ�ʹ��      
    /*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//�ȹر�  //ʹ������  ʹ��TIME2��ʱ��
    TIM_Cmd(TIM2, ENABLE);	// ����ʱ��      
*/
}

//-----------------------------------------------------------------
//----------------------------------------------------------------
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
//��ʱ��TIM3��ʼ��   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//�ж����ȼ�NVIC����
   NVIC_InitTypeDef NVIC_InitStructure; 
    
//��ʱ��TIM3��ʼ��   **************************************************  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
    TIM_DeInit(TIM3);//��λ��ʱ��3���мĴ���

    TIM_TimeBaseStructure.TIM_Period=arr;	//�Զ���װ�ؼĴ������ڵ�ֵ
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	//ʱ��Ԥ��Ƶ�� 72M/72      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
/*λ9:8 CKD[1:0]: ʱ�ӷ�Ƶ���Ӷ����ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)ʹ�õĲ���Ƶ��֮��ķ�Ƶ������*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);	// �������жϱ�־ 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//ʹ��ָ����TIM3�ж�,��������ж�

 //�ж����ȼ�NVIC����   **************************************************     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx		// ����ʱ��    
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);	//�ȹرյȴ�ʹ��      
    /*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);	//�ȹر�  //ʹ������  ʹ��TIME2��ʱ��
    TIM_Cmd(TIM3, ENABLE);	// ����ʱ��      
*/
}

//-----------------------------------------------------------------
//----------------------------------------------------------------
//��ʱ��1�жϷ������
void TIM1_IRQHandler(void)   //TIM1�ж�
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM1�����жϷ������
    {
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
            //////////***//////////////
//         printf(" //��ʱ�� 1 �жϷ������\r\n");            
            //////////***//////////////
	}
}

//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
            //////////***//////////////
//         printf(" //��ʱ�� 2 �жϷ������\r\n");            
            //////////***//////////////
	}
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 

            //////////***//////////////
//         printf(" //��ʱ�� 3 �жϷ������\r\n");  
            //////////***//////////////
	}
}


//����ڶ�
void SG90_out(u16 num)//500-2500
{
	TIM1->CCR1 = num;//pwm���ߵ�ƽ����ʱ�䣬2ms
}
//����ڶ�
void MG90S_out(u16 num)//500-2500
{
	TIM1->CCR2 = num;//pwm���ߵ�ƽ����ʱ�䣬2ms
}

