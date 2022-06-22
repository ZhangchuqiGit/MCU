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
void TIM2_Int_Init(u16 arr,u16 psc)
//   TIM2_Int_Init(1000-1,72-1);
1000-1    72-1  = 1ms 
�ж�����Ϊ1ms
void TIM3_Int_Init(u16 arr,u16 psc)
//  TIM3_Int_Init(5000-1,7200-1);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ50ms 
// �ж�/���  ʱ�� t = ( 72M /(psc+1) ) * ( arr+1 ) = 72M * arr / psc
				   t = ( 72M / 7200 ) * ( 5000 ) = 72M * 5000 / 7200 = 50ms
				   t = ( 72M / 72 ) * ( 20000 ) = 72M * 20 000 / 72 = 20ms
****************/

#if 0

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
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1/* arr */;//��ʱ������,20ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;/* psc *///ʱ��Ԥ��Ƶ
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

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	   //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��   
    NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC�Ĵ���
 
    TIM_Cmd(TIM2, ENABLE);	// ����ʱ��    
    
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//�ȹرյȴ�ʹ��      
/*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//�ȹر�  //ʹ������  ʹ��TIME2��ʱ��
    TIM_Cmd(TIM2, ENABLE);	// ����ʱ��      */
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
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  
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
            //////////***//////////////
//         printf(" //��ʱ�� 1 �жϷ������\r\n");            
            //////////***//////////////
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
}

//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
            //////////***//////////////
//         printf(" //��ʱ�� 2 �жϷ������\r\n");            
            //////////***//////////////
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{

            //////////***//////////////
//         printf(" //��ʱ�� 3 �жϷ������\r\n");  
            //////////***//////////////
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
}

#endif


#if 1

/* ��ʱ��2 ͨ��2 (PA1) ���� PPM �������� */
//TIM2_Capture_PPM_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
void TIM2_Capture_PPM_Init() // ���� ң���� PPM ��ͨ��
{
	EXTI_InitTypeDef EXTI_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//��ʱ��TIM2��ʼ��   
	NVIC_InitTypeDef NVIC_InitStructure; 	//�ж����ȼ�NVIC����
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOAʱ��

	//gpio��ʼ����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

    TIM_DeInit(TIM2);//��λ��ʱ��2���мĴ���
	
	//��ʼ����ʱ��2 TIM2	 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =72; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//���벶���ʼ����
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; //�����ȼ��͵�ƽ��ͬ��ͷ�����������½��ش���
  	//�½��ش���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM_ICInitStructure.TIM_ICFilter = 0x05;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//��ʼ����	
	TIM_Cmd(TIM2, ENABLE);	// ����ʱ��    
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// �������жϱ�־ 
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
}

#else
/* ��ʱ��2 ͨ��2 (PA1) ���� PPM �������� */
//TIM2_Capture_PPM_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
void TIM2_Capture_PPM_Init() // ���� ң���� PPM ��ͨ��
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//��ʱ��TIM2��ʼ��   
	NVIC_InitTypeDef NVIC_InitStructure; 	//�ж����ȼ�NVIC����
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOAʱ��


	//gpio��ʼ����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

    TIM_DeInit(TIM2);//��λ��ʱ��2���мĴ���
	
	//��ʼ����ʱ��2 TIM2	 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =72; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//���벶���ʼ����
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; //�����ȼ��͵�ƽ��ͬ��ͷ�����������½��ش���
  	//�½��ش���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM_ICInitStructure.TIM_ICFilter = 0x05;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//��ʼ����	
	TIM_Cmd(TIM2, ENABLE);	// ����ʱ��    
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// �������жϱ�־ 
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
}
#endif


/**/
u32 temp=0;
u8  TIM2_CH2_capture_state=0,ppm_rx_sta=0,ppm_rx_num=0;	//���벶��״̬		    				
u16	TIM2_CH2_capture_value;	//���벶��ֵ
u16 ppm_rx[10];//ppm_rx[0]   1   ���յ�ppm����

//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{
 	if( (TIM2_CH2_capture_state&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)		 
		{	    
			if(TIM2_CH2_capture_state&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM2_CH2_capture_state&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM2_CH2_capture_state|=0X80;//��ǳɹ�������һ��
					TIM2_CH2_capture_value=0XFFFF;
				}
				else TIM2_CH2_capture_state++;
			}	 
		}
		if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM2_CH2_capture_state&0X40)		//����һ���½��� 		
			{	  			
				TIM2_CH2_capture_state|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM2_CH2_capture_value=TIM_GetCapture1(TIM2);
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM2_CH2_capture_state=0;			//���
				TIM2_CH2_capture_value=0;
				TIM_SetCounter(TIM2,0);
				TIM2_CH2_capture_state|=0X40;		//��ǲ�����������
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
	
	/* ����֡���� */
	if( (TIM2_CH2_capture_state&0X80)==1)//�ɹ�������һ��������
	{
//			temp=TIM2_CH2_capture_state&0X3F;
//			temp*=65536;//���ʱ���ܺ�
//			temp+=TIM2_CH2_capture_value;//�õ��ܵĸߵ�ƽʱ��	
		if(ppm_rx_sta==1) {ppm_rx[ppm_rx_num+1]=TIM2_CH2_capture_value;ppm_rx_num++;}//printf("TIM2_CH2_capture_value:%d\r\n",TIM2_CH2_capture_value);
		if(4>TIM2_CH2_capture_state&0X3F>0||TIM2_CH2_capture_value>3000) ppm_rx_sta++;//�͵�ƽʱ�����3000usΪ��ʼ֡
		if(ppm_rx_sta==2) {ppm_rx_sta=0;ppm_rx[0]=1;ppm_rx_num=0;}//printf("receive\r\n");//ppm_rx_sta   1 ��ʾ���յ�ͬ��֡/ 2���յ�����һ��ʼ֡ ppm���ݽ������
		
		TIM2_CH2_capture_state=0;//������һ�β���
		
	}
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}



//----------------------------------------------------------------
// ��� �ڶ�
/* ���� TIM4 ����� PWM �źŵ�ģʽ�������ڡ����ԡ�ռ�ձ� */
void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
	u16 CCR1_Val = 0;        
	u16 CCR2_Val = 0;
	u16 CCR3_Val = 0;
	u16 CCR4_Val = 0;

/* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;//��ʱ������,20ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;//ʱ��Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//������ʽ�����ϼ���
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

/* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//ѡ��ʱ��ģʽ��tim�������2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//����
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

/* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	
	
/* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	

/* PWM1 Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR4_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	

	/**/
    TIM_ARRPreloadConfig(TIM4, ENABLE);

    /* TIM1 enable counter */
    TIM_Cmd(TIM4, ENABLE);
    TIM_CtrlPWMOutputs(TIM4, ENABLE);	
}

void TIM4_Int_Init()/* ����TIM1�������PWMʱ�õ���I/O  */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//��ʱ��1ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIOBʱ��ʹ��
	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*���úͳ�ʼ��TIM1 */
void TIM4_PWM_SG90_MG90_Init(void)
{
	TIM4_Int_Init();
	TIM4_Mode_Config();	
}

	
//����ڶ�
void SG90_MG90_1(u16 num)//500-2500
{
	TIM4->CCR1 = num;//pwm���ߵ�ƽ����ʱ�䣬2ms
}
//����ڶ�
void SG90_MG90_2(u16 num)//500-2500
{
	TIM4->CCR2 = num;//pwm���ߵ�ƽ����ʱ�䣬2ms
}
//����ڶ�
void SG90_MG90_3(u16 num)//500-2500
{
	TIM4->CCR3 = num;//pwm���ߵ�ƽ����ʱ�䣬2ms
}
//����ڶ�
void SG90_MG90_4(u16 num)//500-2500
{
	TIM4->CCR4 = num;//pwm���ߵ�ƽ����ʱ�䣬2ms
}
