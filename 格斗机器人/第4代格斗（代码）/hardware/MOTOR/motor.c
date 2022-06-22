#include "motor.h"
/***********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File��TB6612��������
Author��pinggai    Version:1.0     Data:2015/05/06
Description: Balance-STM32 TB6612FNG��������
             
			 PC0~3��AIN1,AIN2,BIN1,BIN2;������ƶ˿�        //PA0~3
			 PA6��PWMA;�ҵ����J1���ٶȿ��� 
			 PA7��PWMB;������J2���ٶȿ���
             
			 371�����������TB6612FNG���ӷ�ʽ			
	         �ҵ����J1������ɫ��AO1;��ɫ����AO2
             ������J2������ɫ��BO1;��ɫ����BO2
	         
			 ��ǰ�����Ҳ࿴�������˳ʱ�뷽��ת��  ��ת
	         ��󣺴��Ҳ࿴���������ʱ�뷽��ת��  ��ת  
***********************************************************/

//�����ת��������źŶ˿ڳ�ʼ��
//PC0~3��������������
//void M_Init(void)
//{
//	RCC->APB2ENR|=1<<4;      //ʹ��PORTCʱ��
//	GPIOC->CRL&=0XFFFF0000;
//	GPIOC->CRL|=0X00003333;  //PC3~0�������
//	GPIOC->ODR|=0XF<<0;	     //PC3~0����ߵ�ƽ
//}
void M_Init(void)
{
	GPIO_InitTypeDef GPIO_Initsturct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Initsturct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init(GPIOA,&GPIO_Initsturct);
	
//	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_6;
//  GPIO_Init(GPIOA,&GPIO_Initsturct);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
//	GPIO_SetBits(GPIOA,GPIO_Pin_6);
}

//��ʱ��TIM3��PWM���Ƴ�ʼ��,CH1��PA6����CH2(PA7)��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//����TIM3_PWM_Init(250,0); 
//�趨�Զ�װ��ֵΪ250������Ƶ��PWMƵ��=72000/250=80Khz
void TIM3_PWM_Init(u16 arr,u16 psc) //arr�趨�������Զ���װֵ   
                                    //pscԤ��Ƶ������Ƶ,psc=0
{
	RCC->APB1ENR|=1<<1;       //TIM3ʱ��ʹ��    
	  	
	GPIOA->CRL&=0X00FFFFFF;//PA6,7���
	GPIOA->CRL|=0XBB000000;//���ù������ 	  
	GPIOA->ODR|=3<<6;//PA6.7����	

	TIM3->ARR=arr;//�趨�������Զ���װֵ 
	TIM3->PSC=psc;//Ԥ��Ƶ������Ƶ
	
	TIM3->CCMR1|=6<<4;  //CH1 PWM1ģʽ	�ߵ�ƽ��Ч	 
	TIM3->CCMR1|=1<<3; //CH1Ԥװ��ʹ��
	TIM3->CCMR1|=6<<12;  //CH2 PWM1ģʽ		 
	TIM3->CCMR1|=1<<11; //CH2Ԥװ��ʹ��	   

	TIM3->CCER|=1<<0;   //OC1 ���ʹ��
	TIM3->CCER|=1<<4;   //OC2 ���ʹ��	   

	TIM3->CR1=0x0080;   //ARPEʹ�� 
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3 
}
/**************        *****************  ���  ����  *****************          *****************/
void Motor__Control(s16 speed  , s16 swerve/*ת����*/)//speed:ǰ��/����   swerve:��ת/��ת 
{
//	printf("�ֱ�����������\r\n");
	if( speed >= +1 )//ǰ�� 0~46
	{
		if( swerve >= +1 )//��תswerve: 0~50
		{
			M1_1 = 1;
			M1_2 = 0;	
			TIM3->CCR1 = speed>swerve ? speed : swerve  ;		
			M2_1 = 1;
			M2_2 = 0;			
			TIM3->CCR2 = speed>swerve ? speed-swerve/2 : swerve*65/100 ;//75-52/2=75-26=49= 65%
		}
		else if( swerve <= -1 )//��תswerve: -50~0
			{
				swerve=-swerve;//ȡ����
				M1_1 = 1;
				M1_2 = 0;	
				TIM3->CCR1 = speed>swerve ? speed-swerve/2 : swerve*65/100 ;//75-52/2=75-26=49= 65%
				M2_1 = 1;
				M2_2 = 0;			
				TIM3->CCR2 = speed>swerve ? speed : swerve  ;	
			}
			else //ǰ��speed: 0~46  
			{
				M1_1 = 1;
				M1_2 = 0;	
				TIM3->CCR1 = speed ;	
				M2_1 = 1;
				M2_2 = 0;			
				TIM3->CCR2 = speed;		
			}
	}
	else if( speed <= -1 )//����
		{
			speed=-speed;//ȡ����
			if( swerve >= +1 )//��תswerve: 0~50
			{
				M1_1 = 0;
				M1_2 = 1;	
				TIM3->CCR1 = speed>swerve ? speed : swerve  ;	
				M2_1 = 0;
				M2_2 = 1;			
				TIM3->CCR2 = speed>swerve ? speed-swerve/2 : swerve*65/100 ;//75-52/2=75-26=49= 65%
			}
			else if( swerve <= -1 )//��תswerve: -50~0
				{		
					swerve=-swerve;//ȡ����
					M1_1 = 0;
					M1_2 = 1;	
					TIM3->CCR1 =  speed>swerve ? speed-swerve/2 : swerve*65/100 ;//75-52/2=75-26=49= 65%
					M2_1 = 0;
					M2_2 = 1;			
					TIM3->CCR2 =  speed>swerve ? speed : swerve ;
				}
				else //����speed: 0~46
				{
					M1_1 = 0;
					M1_2 = 1;	
					TIM3->CCR1 = speed ;	
					M2_1 = 0;
					M2_2 = 1;			
					TIM3->CCR2 = speed;		
				}
		}
		else //ԭ�ش�ת--�Ȳ�ǰ��Ҳ������
		{
			if( swerve >= +1 )//ԭ����תswerve: 0~46
			{
				M1_1 = 1;
				M1_2 = 0;	
				TIM3->CCR1 = swerve;	
				M2_1 = 0;
				M2_2 = 1;			
				TIM3->CCR2 = swerve;		
			}
			else if( swerve <= -1 )//ԭ����תswerve: -46~0
				{
					swerve=-swerve;//ȡ����
					M1_1 = 0;
					M1_2 = 1;	
					TIM3->CCR1 = swerve ;	
					M2_1 = 1;
					M2_2 = 0;			
					TIM3->CCR2 = swerve;		
				}
				else
				{
					M1_1 = 1;
					M1_2 = 1;
					M2_1 = 1;
					M2_2 = 1;
					TIM3->CCR1 = 255;
					TIM3->CCR2 = 255;
				}
		}	
}

#if 0
//���ҵ���������ٶȿ���
//motor1���ҵ����J1����motor2��������J2��
//С��0ʱ����ǰ������0ʱ�����    
//motor1/2��ȡֵ��Χ��-250~+250����ֵ�Ĵ�С����ռ�ձȵĴ�С
//��motor1ȡֵΪ90����ռ�ձ�Ϊ10%��
void Motor_Speed_Control(s16 motor1, s16 motor2)	 
{
    s16 motor1speed = 0, motor2speed = 0 ;	
    if(motor1>250)  motor1speed = 250;
	    else if (motor1<-250)  motor1speed = -250;
			else  motor1speed = motor1;
	if(motor2>250)  motor2speed = 250;
	    else if (motor2<-250)  motor2speed = -250;
			else  motor2speed = motor2;
/**********************************************/
	if(motor1speed == 0) //ɲ��
	{
		M1_1 = 1;
		M1_2 = 1;
		TIM3->CCR1 = 250;
	}
	else if(motor1speed > 0)
		{
			M1_1 = 1;
			M1_2 = 0;
			TIM3->CCR1 = motor1speed;
		}
		else
		{
			M1_1 = 0;
			M1_2 = 1;
			TIM3->CCR1 = -motor1speed;
		}
/**********************************************/
	if(motor2speed == 0)//ɲ��
	{
		M2_1 = 1;
		M2_2 = 1;
		TIM3->CCR2 = 250;
	}
	else if(motor2speed > 0)
		{
			M2_1 = 1;
			M2_2 = 0;
			TIM3->CCR2 = motor2speed;
		}
		else
		{
			M2_1 = 0;
			M2_2 = 1;
			TIM3->CCR2 = -motor2speed;
		}
}

///*************************************************��ʱ��*******************************************************/
////ͨ�ö�ʱ��2�жϳ�ʼ��
////����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M=72M
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
//void TIM2_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
//	NVIC_InitTypeDef 			NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
//    TIM_DeInit(TIM2);//��λ��ʱ��2���мĴ��� 

//	//��ʱ��TIM2��ʼ��
//	TIM_TimeBaseStructure.TIM_Period = arr; 					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 					//ʹ��ָ����TIM2�ж�,��������ж�

//	//�ж����ȼ�NVIC����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//��ռ���ȼ�1��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  							//��ʼ��NVIC�Ĵ���

//	TIM_Cmd(TIM2, ENABLE);  									//ʹ��TIM2					 
//}

///* **************** STM32F103C8T6 ��ʱ���жϷ������**************** */
//void TIM2_IRQHandler(void)
//{
//}

#endif
