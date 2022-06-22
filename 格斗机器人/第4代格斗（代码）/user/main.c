#include <stm32f10x.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"	
#include "pstwo.h"
#include "motor.h"
#include "math.h"
#include "stdlib.h"

#define speed_max 250
/*****************************************/
u8 flag_red=0 , flywheel=0 , flag_delay1=0 ;   
s16 speed =0 , swerve/*ת����*/=0 ;

/*****************************************/
void speed_select(void);//�õ�һ��ҡ�˵�ģ����  ��Χ0~256 
void stop_fast(void);
void stop_init(void); 

/*********         ***********      main        ***********             *********/
int main(void)
{
	SystemInit();//ϵͳʱ������
	delay_init();	     //��ʱ��ʼ��
	uart_init(115200);  //����1��ʼ�� 
/*  �ж�/���  ʱ�� Tout us=   250 * 72 / 72 = 250 us  */
	TIM3_PWM_Init(speed_max-1,72-1);
	LED_Init();
	PS2_Init();			 //�����˿ڳ�ʼ��
	PS2_SetInit();		 //�����ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸� //������ģʽ
	M_Init();	   //�����ת��������źŶ˿ڳ�ʼ��	
    stop_init( );
	while(1)
	{	
		if(flag_delay1>=110)/*11ms*/
		{	
			flag_delay1=0;
			if( PS2_RedLight() ==0 )//����ֵ0-���ģʽ
			{			
				delay_ms(4); /*4ms!!!!!*/ 
				LED = 0;//�ж��ֱ��Ƿ�Ϊ���ģʽ���ǣ�ָʾ��LED����
				flywheel = PS2_DataKey(); //�ֱ����������� 
				speed_select();//�õ�һ��ҡ�˵�ģ����  ��Χ0~256 				
				Motor__Control(speed,swerve/*ת����*/);//���  ����  //speed:ǰ��/����   swerve:��ת/��ת 
				switch(flywheel)
				{
					case PSB_L1: F_1 = 0;F_2 = 1;break;
					case PSB_L2: F_2 = 0;break;
//					case PSB_L3: PS2_Vibration(0xbf,0x00);/*�����𶯺��������ʱdelay_ms(1000)*/;break;
					case PSB_R1: F_2 = 0;F_1 = 1;break;
					case PSB_R2: F_1 = 0;break;
//					case PSB_R3: PS2_Vibration(0x00,0xbf);/*�����𶯺��������ʱdelay_ms(1000)*/;break;
					default: break;
				}
			}
			else	//�ж��ֱ����Ǻ��ģʽ
			{
				LED = 1;
				stop_init( );
			}
		}
		if( speed==0 && swerve==0 )stop_fast();
//		if(flywheel==PSB_PAD_UP||flywheel==PSB_PAD_RIGHT||flywheel==PSB_PAD_DOWN||flywheel==PSB_PAD_LEFT)
//		{
//			F_1 = 0;
//			F_2 = 0;
//		}
		flag_delay1++;
		delay_us(100);//��ʱ
	}	 
}

/**************        *****************  �õ�һ��ҡ�˵�ģ����  ��Χ0~256  *****************          *****************/
void speed_select(void)//�õ�һ��ҡ�˵�ģ����  ��Χ0~256 
{	 
//	printf("speed_select\r\n");
	speed = -( PS2_AnologData(PSS_LY)-127 ); //�������ˣ�  ����ǰ��
//	printf("��Χ0~125  %d\t",speed);
	if(flywheel == PSB_L3)
	{	/* ����  125*80/35=285  */
		if(speed> 2)speed = (speed-2)*80/35  ; //�������ˣ�
		else if(speed< -2)speed = (speed+2)*80/35 ; //����ǰ��
			 else speed=0; 
	}
	else 
	{	/* ����  125*20/33= 75 */
		if(speed> 2)speed = (speed-2)*20/33; //�������ˣ�
		else if(speed< -2)speed = (speed+2)*20/33; //����ǰ��
			 else speed=0;
	}
//	printf("speed  %d\t",speed);
	swerve = -( PS2_AnologData(PSS_RX)-128 ); //������ת��  ������ת
//	printf("��Χ0~126  %d\t",swerve);
	if(flywheel == PSB_R3)
	{	/* ���� 125*40/19=263 */
		if(swerve> 2)swerve = (swerve-2)*40/19 ;  //������ת 
		else if(swerve< -2)swerve = (swerve+2)*40/19 ; //������ת
			 else swerve=0;	
	}
	else
	{	/* ����  125*20/48=52 */
		if(swerve> 2)swerve = (swerve-2)*20/48; //������ת 
		else if(swerve< -2)swerve = (swerve+2)*20/48; //������ת
			 else swerve=0;	
	}
//	speed = -(PS2_AnologData(PSS_LY)-127)*7;	//����ǰ����  ��������
//	swerve = -(PS2_AnologData(PSS_RX)-127)*7;	//������ת��  ������ת
}

/***********************************************************************************************/
void stop_fast(void)//
{
	M1_1 = 1;
	M1_2 = 1;
	TIM3->CCR1 = 255;
	M2_1 = 1;
	M2_2 = 1;
	TIM3->CCR2 = 255;
}
void stop_init(void)//
{
	M1_1 = 0;
	M1_2 = 0;
	M2_1 = 0;
	M2_2 = 0;
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	F_2 = 0;
	F_1 = 0;
} 

