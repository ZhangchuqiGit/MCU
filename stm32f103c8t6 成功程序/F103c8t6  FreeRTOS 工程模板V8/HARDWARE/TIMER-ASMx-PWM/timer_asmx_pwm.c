/***************STM32F103C8T6**********************/
//�ӿ� ��TIM1, CH1-PB13, CH2-PB14, //CH3-PB15
#include "timer_asmx_pwm.h"

/*        *****************************************************************
����Ŀ���һ����Ҫһ��20ms ��50hz�����ҵ�ʱ�����壬������� �� ��ƽ����һ��Ϊ0.5ms-2.5ms��Χ�ڵĽǶȿ������岿�֣�
�ܼ��Ϊ2ms����180�ȽǶ��ŷ�Ϊ������ô��Ӧ�Ŀ��ƹ�ϵ�������ģ�
   0.5ms--------------0�ȣ�
   1.0ms------------45�ȣ�
   1.5ms------------90�ȣ�
   2.0ms-----------135�ȣ�
   2.5ms-----------180�ȣ�            ����stm32��˵�����ǿ���ʹ�ö�ʱ��PWM��������ƶ����
*******************************************************   
PWM�����ʼ��  arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ��     
// �ж�/���  ʱ�� Tout us= ((arr+1)*(psc+1))/Tclk
����ռ�ձ�= pwmval/10000 //1.5ms->90��   pwmval1=250-1;//500ms,   500-2500ms
	PWM���Ƶ�ʵļ��㣺
	PWM�������һ�������źţ��źŵ�Ƶ������TIMx��ʱ��Ƶ�ʺ�TIMx_ARR����Ĵ�����������,����źŵ�ռ�ձ�������TIMx_CRRx�Ĵ���ȷ��:
	ռ�ձ�=(TIMx_CRRx/TIMx_ARR�Զ���װ�ؼĴ���)*100%    
	*/
void TIM1_PWM_Init(u16 arr,u16 psc)  //����Ŀ���
//void TIM1_PWM_Init(void)  //����Ŀ���
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure;  
    TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  		TIM_OCInitStructure;  
/**/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//��ʱ��1ʱ��ʹ��//GPIOBʱ��ʹ��
/**/ 
    TIM_DeInit(TIM1);//��λ��ʱ��1���мĴ��� 
//���ø�����Ϊ�����������,���PWM���岨��  
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 ; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  /*----++++++++++++++++------*/
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //���츴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);
/* Time base configuration */
// �ж�/���  ʱ�� Tout us= ((arr+1)*(psc+1))/Tclk (72) =20000 *72 /72 =20 000us=20ms
    TIM_TimeBaseStructure.TIM_Period = arr;//arr��ʱ������,20ms
//	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;//pscʱ��Ԥ��Ƶ
//	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //����ʱ�ӷָ�:TDTS = Tck_tim,TIM_CKD_DIV1 =0x0
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//������ʽ�����ϼ���
//��ʼ����ʱ��
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ  
/*		   ---------	    	��ʼ��TIMx Channelx PWMģʽ				------------		 */
// PWM1 Mode configuration: Channel1 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//ѡ��ʱ��ģʽ��tim�������2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0 ;//****  CCR1_Val=0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//����
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH1Ԥװ��ʹ��
/* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0 ;//****  CCR2_Val=0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//����
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//������ͨ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH2Ԥװ��ʹ��	
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
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH3Ԥװ��ʹ��		*/
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���  
	TIM_CtrlPWMOutputs(TIM1,ENABLE);    //MOE �����ʹ��  
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM4  
} 	
 

//����ڶ�
void SG90_out(u16 num)//500-2500  --  20 000
{
	TIM1->CCR1 = num;//pwm���ߵ�ƽ����ʱ�䣬2ms
}
//����ڶ�
void MG90S_out(u16 num)//500-2500  --  20 000
{
	TIM1->CCR2 = num;//pwm���ߵ�ƽ����ʱ�䣬2ms
}

/*************************************************��ʱ��*******************************************************/
//ͨ�ö�ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M=72M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
    TIM_DeInit(TIM2);//��λ��ʱ��2���мĴ��� 

	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 					//ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  							//��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  									//ʹ��TIM2					 
	TIM_Cmd(TIM2, DISABLE);  						 
}
/*************************************************��ʱ��*******************************************************/
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
    TIM_DeInit(TIM3);//��λ��ʱ��3���мĴ��� 
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 						//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );						 //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  				//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  		//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  			//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  								//��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3					 
}
/***********************************************��ʱ��*********************************************************/
//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
    TIM_DeInit(TIM4);//��λ��ʱ��4���мĴ��� 
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 			//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 					//ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  			//TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//��ռ���ȼ�5��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  							//��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  									//ʹ��TIM4					 
}

/* **************** STM32F103C8T6 ��ʱ���жϷ������**************** */
u8 s_val=0;
u8 s=2;
u8 Flag=0;
u16 i=1200;//��ͷ
u16 j=1600;//ҡͷ
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		if(Flag==0)
		{
			i-=10;
			j-=10;
		}
		if(Flag==1)
		{
			i+=10;
			j+=10;
		}
		if(s_val==1)//��ͷ
		{
			if(i<=950)
			{
				Flag=1;
			}
			if(i>=1450)
			{
				Flag=0;
			}
			if(i==1200)s--;
			SG90_out(i);//��500-2500��   PB 13
		}
		else if(s_val==2)//ҡͷ
			{
				if(j<=1300)
				{
					Flag=1;
				}
				if(j>=1900)
				{
					Flag=0;
				}
				if(j==1600)s--;
				MG90S_out(j);//��500-2500��  PB 14
			}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
	if(s==0)
	{		
		if(s_val==2)MG90S_out(1600); 
		else if(s_val==1 )SG90_out(1200); 
		s_val=0;
		TIM_Cmd(TIM2, DISABLE); 
	}				 

}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		printf("TIM 3 ���.......\r\n");
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
		printf("TIM 4 ���.......\r\n");
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}

