#include "motor.h"

u8 flywheel=0;

void motor_Init(void)//������ƶ˿ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
//	JTAG_SWD_GPIO_Config();//��ʹ��JTAG���ԣ���Ӧ��IO����PB3,PB4,PA15������Ϊ��ͨIO��ʹ�ã�������ʹ��SWD�������¼��������ͷų���

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_5);  //���� 0
	
	PWMA(0); //�����ٶȿ��� CH1(PA6)
	PWMB(0); //�ҵ���ٶȿ��� CH2(PA7)
}

void PWMA(u16 speedval)/*�����ٶȿ��� CH1(PA6)*/
{
	TIM3->CCR1=speedval;//�����ٶȿ��� CH1(PA6) //PWMA()
}
void PWMB(u16 speedval)/*�����ٶȿ��� CH2(PA7)*/
{
	TIM3->CCR2=speedval;//�ҵ���ٶȿ��� CH2(PA7) //PWMB()
}

/*	TIM3_PWM_Init (arr, psc);   
����TIM3_PWM_Init (1000,144);    �趨�Զ�װ��ֵΪ1000����Ƶ144��     
�ж�/���  ʱ�� Tout us= (arr*psc)/Tclk
�ж�/���  ʱ�� Tout us=  1000 * 144 / (72 MHz) = 2000 us  */
//��ʱ��TIM3��PWM���Ƴ�ʼ��,CH1��PA6����CH2(PA7)��
//arr��rr�趨�������Զ���װֵ
//psc��ʱ��Ԥ��Ƶ����Ԥ��Ƶ��
//����TIM3_PWM_Init(250,0); 
//�趨�Զ�װ��ֵΪ250������Ƶ��PWMƵ��=72000/250=80Khz
void TIM3_PWM_Init(u16 arr,u16 psc) 
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
	
	TIM3->CCR1=0;//�����ٶȿ��� CH1(PA6) //PWMA()
	TIM3->CCR2=0;//�ҵ���ٶȿ��� CH2(PA7) //PWMB()
}

#if 1
void Motor_Control(s16 speed, s16 swerve)//speed:ǰ��/���� , swerve:��ת/��ת 
{
	u8 sweval=20;
	speed=speed*7.0/10.0;/* ���ٱ� *///�������ˣ�  ����ǰ��

	if( speed >= +1 )//ǰ��
	{
		if(flywheel==PSB_L3)
		{
			speed=speed*10.0/7.0;/* ���ٱ� */
			//speed=speed*7.0/10.0;/* ���ٱ� */
		}
		MR_GO = 1;
		MR_BACK = 0;	
		ML_GO = 1;
		ML_BACK = 0;			
		if( swerve >= +1 )//��ת
		{
			PWMA(speed);/*�����ٶȿ��� CH1(PA6)*/
			PWMB(	((100.0-swerve)<=sweval)?  sweval:(speed*(100.0-swerve)/100.0)	);
		}
		else if( swerve <= -1 )//��ת
			{
				swerve=-swerve;//ȡ����
				PWMA(((100.0-swerve)<=sweval)?  sweval:(speed*(100.0-swerve)/100.0)); 				
				PWMB(speed); 
			}
			else //ǰ��
			{
				PWMA(speed);
				PWMB(speed); 
			}
	}
	else if( speed <= -1 )//����
		{
			speed=-speed;//ȡ����
			MR_GO = 0;
			MR_BACK = 1;	
			ML_GO = 0;
			ML_BACK = 1;			
			if( swerve >= +1 )//��ת
			{
				PWMA(speed); 
				PWMB(((100.0-swerve)<=sweval)? sweval:(speed*(100.0-swerve)/100.0));				
			}
			else if( swerve <= -1 )//��ת
				{		
					swerve=-swerve;//ȡ����
					PWMA(((100.0-swerve)<=sweval)?  sweval:(speed*(100.0-swerve)/100.0)); 				
					PWMB(speed); 
				}
				else //����
				{
					PWMA(speed);
					PWMB(speed); 
				}
		}
		else //ԭ�ش�ת--�Ȳ�ǰ��Ҳ������
		{
			if( swerve >= +1 )//ԭ����ת
			{
				MR_GO = 0;
				MR_BACK = 1;	
				ML_GO = 1;
				ML_BACK = 0;	
				if(flywheel==PSB_R3) 
				{
					PWMA(swerve);
					PWMB(swerve); 
				}
				else 
				{
					PWMA(swerve*70.0/100.0);
					PWMB(swerve*70.0/100.0); 
				}
			}		
			else if( swerve <= -1 )//ԭ����ת
				{
					swerve=-swerve;//ȡ����
					MR_GO = 1;
					MR_BACK = 0;	
					ML_GO = 0;
					ML_BACK = 1;			
					if(flywheel==PSB_R3) 
					{
						PWMA(swerve);
						PWMB(swerve); 
					}
					else 
					{
						PWMA(swerve*70.0/100.0);
						PWMB(swerve*70.0/100.0); 
					}
				}
				else //speed=0,swerve=0
				{
					stop();
				}
		}	
}
#else
void Motor_Control(s16 speed, s16 swerve)//speed:ǰ��/���� , swerve:��ת/��ת 
{
//	printf("�ֱ�����������\r\n");
	if( speed >= +1 )//ǰ��
	{
		if( swerve >= +1 )//��ת
		{
			if(speed_max<=speed*(100.0-swerve)/100.0)			
			{
				MR_GO = 1;
				MR_BACK = 0;	
			}
			else
			{
				MR_GO = 0;
				MR_BACK = 0;	
			}
			if(speed_max<=speed)		
			{
				ML_GO = 1;
				ML_BACK = 0;			
			}
			else
			{
				ML_GO = 0;
				ML_BACK = 0;			
			}
		}
		else if( swerve <= -1 )//��ת
			{
				swerve=-swerve;//ȡ����
				if(speed_max<=speed)				
				{
					MR_GO = 1;
					MR_BACK = 0;	
				}
				else
				{
					MR_GO = 0;
					MR_BACK = 0;	
				}
				if(speed_max<=speed*(100.0-swerve)/100.0)	
				{
					ML_GO = 1;
					ML_BACK = 0;			
				}
				else
				{
					ML_GO = 0;
					ML_BACK = 0;			
				}
			}
			else //ǰ��
			{
				MR_GO = 1;
				MR_BACK = 0;	
				ML_GO = 1;
				ML_BACK = 0;			
			}
		}
		else if( speed <= -1 )//����
		{
			speed=-speed;//ȡ����
			if( swerve >= +1 )//��ת
			{
				if(speed_max<=speed*(100.0-swerve)/100.0)			
				{
					MR_GO = 0;
					MR_BACK = 1;	
				}
				else
				{
					MR_GO = 0;
					MR_BACK = 0;	
				}
				if(speed_max<=speed )			
				{
					ML_GO = 0;
					ML_BACK = 1;			
				}
				else
				{
					ML_GO = 0;
					ML_BACK = 0;			
				}
			}
			else if( swerve <= -1 )//��ת
				{		
					swerve=-swerve;//ȡ����
					if(speed_max<=speed)			
					{
						MR_GO = 0;
						MR_BACK = 1;	
					}
					else
					{
						MR_GO = 0;
						MR_BACK = 0;	
					}
					if(speed_max<=speed*(100.0-swerve)/100.0 )			
					{
						ML_GO = 0;
						ML_BACK = 1;			
					}
					else
					{
						ML_GO = 0;
						ML_BACK = 0;			
					}
				}
				else //����
				{
					MR_GO = 0;
					MR_BACK = 1;	
					ML_GO = 0;
					ML_BACK = 1;			
				}
		}
		else //ԭ�ش�ת--�Ȳ�ǰ��Ҳ������
		{
			if( swerve >= +1 )//ԭ����ת
			{
				if(flywheel==PSB_R3) 
				{
					if(speed_max<= swerve)			
					{
						MR_GO = 0;
						MR_BACK = 1;	
						ML_GO = 1;
						ML_BACK = 0;			
					}
					else 		
					{
						MR_GO = 0;
						MR_BACK = 0;	
						ML_GO = 0;
						ML_BACK = 0;			
					}
				}
				else 
				{
					if(speed_max<= swerve*6.0/8.0)			
					{
						MR_GO = 0;
						MR_BACK = 1;	
						ML_GO = 1;
						ML_BACK = 0;			
					}
					else 		
					{
						MR_GO = 0;
						MR_BACK = 0;	
						ML_GO = 0;
						ML_BACK = 0;			
					}
				}
			}
			else if( swerve <= -1 )//ԭ����ת
				{
					swerve=-swerve;//ȡ����
					if(flywheel==PSB_R3) 
					{
						if(speed_max<= swerve)			
						{
							MR_GO = 1;
							MR_BACK = 0;	
							ML_GO = 0;
							ML_BACK = 1;			
						}
						else 		
						{
							MR_GO = 0;
							MR_BACK = 0;	
							ML_GO = 0;
							ML_BACK = 0;			
						}
					}
					else 
					{
						if(speed_max<= swerve*6.0/8.0)			
						{
							MR_GO = 1;
							MR_BACK = 0;	
							ML_GO = 0;
							ML_BACK = 1;			
						}
						else 		
						{
							MR_GO = 0;
							MR_BACK = 0;	
							ML_GO = 0;
							ML_BACK = 0;			
						}
					}
				}
				else //speed=0,swerve=0
				{
					stop();
				}
		}	
}
#endif 

void stop(void)//ɲ��
{
	MR_GO = 0;
	MR_BACK = 0;
	ML_GO = 0;
	ML_BACK = 0;
	PWMA(0);
	PWMB(0);
}

void stop_Measure(void)//ɲ��
{
	MR_GO = 0;
	MR_BACK = 1;
	ML_GO = 0;
	ML_BACK = 1;
	PWMA(speed_max);
	PWMB(speed_max);
}
void stop_Measure_B(void)//ɲ��
{
	MR_GO = 1;
	MR_BACK = 0;
	ML_GO = 1;
	ML_BACK = 0;
	PWMA(speed_max);
	PWMB(speed_max);
}

