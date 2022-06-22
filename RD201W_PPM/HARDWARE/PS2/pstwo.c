/*PS2�ֱ���������*/	 
#include "pstwo.h"

s16 speed=0,swerve=0;	  

u16 Handkey;
u8 Comd[2]={0x01,0x42};	//��ʼ�����������
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//����ֵ�밴����

//���ֱ���������
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //�����Ϊ����λ
		}
		else DO_L;

		CLK_H;                        //ʱ������
		delay_us(50);
		CLK_L;
		delay_us(50);
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
}
//�ж��Ƿ�Ϊ���ģʽ
//����ֵ��0�����ģʽ
//		  ����������ģʽ
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//��ȡ�ֱ�����
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;

	CS_L;

	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������

	for(byte=2;byte<9;byte++)          //��ʼ��������
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			CLK_L;
			delay_us(50);
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(50);
	}
	CS_H;	
}

//�Զ�������PS2�����ݽ��д���      ֻ�����˰�������         Ĭ�������Ǻ��ģʽ  ֻ��һ����������ʱ
//����Ϊ0�� δ����Ϊ1
u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //û���κΰ�������
}

//�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}

//������ݻ�����
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}
/******************************************************
Function:    void PS2_Vibration(u8 motor1, u8 motor2)
Description: �ֱ��𶯺�����
Calls:		 void PS2_Cmd(u8 CMD);
Input: motor1:�Ҳ�С�𶯵�� 0x00�أ�������
	   motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
******************************************************/
void PS2_Vibration(u8 motor2, u8 motor1)
{
	CS_L;
	delay_us(16);
    PS2_Cmd(0x01);  //��ʼ����
	PS2_Cmd(0x42);  //��������
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}
//short poll
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}
//��������
void PS2_EnterConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//����ģʽ����
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  �������÷���ģʽ
	PS2_Cmd(0xEE); //Ox03�������ã�������ͨ��������MODE������ģʽ�� 
				   //0xEE�������������ã���ͨ��������MODE������ģʽ��
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//������
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}
//��ɲ���������
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
//�ֱ����ó�ʼ��
void PS2_SetInit(void)//�����˿ڳ�ʼ�������á����̵�ģʽ������ѡ���Ƿ�����޸ģ�������ģʽ
{
	PS2_Init();//�����˿ڳ�ʼ��
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//��������ģʽ
	PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
	PS2_VibrationMode();	//������ģʽ
	PS2_ExitConfing();		//��ɲ���������
}

//*  DI->PB10   DO->PB0    CS->PB1       CLK->PB2      */
void PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_Initsturct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
    //����  DI->PB10
	GPIOB->CRH&=0XFFFFF0FF;//PB10���ó�����	Ĭ������  
	GPIOB->CRH|=0X00000800;   
    //  DO->PB13    CS->PB14       CLK->PB15
	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��  	   	  	 
	GPIOB->CRH&=0X000FFFFF; 
	GPIOB->CRH|=0X33300000;//PB13��PB14��PB15 �������   	 											  
	
	GPIO_Initsturct.GPIO_Mode=GPIO_Mode_IPD;// ��������
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_0; 
	//  DO->PB0    CS->PB1       CLK->PB2
	GPIO_Init(GPIOB,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Mode=GPIO_Mode_Out_PP;//�������   	 
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; 
	//  DO->PB0    CS->PB1       CLK->PB2
	GPIO_Init(GPIOB,&GPIO_Initsturct);
}

/*--------------------------------------------------------------------------------*/
void speed_select(void)//�õ�һ��ҡ�˵�ģ����  ��Χ0~256  ;  �ٶ�: speed -100/+100 ; swerve -100/+100 ;
{		
	speed = -( PS2_AnologData(PSS_LY)-127 ); //�������ˣ�  ����ǰ��
	printf("speed  %d \n\t",speed);
	if(speed> 2)
	{
		speed = (speed-2)*100/125; //�������ˣ�
		if(speed>speed_max)speed=speed_max;
	}
	else if(speed< -2)
		 {
			speed = (speed+2)*100/125; //����ǰ��
			if(speed<-speed_max)speed=-speed_max;
		 }
		 else speed=0;
//	printf("speed  %d\t\n",speed);
		 
	swerve = -( PS2_AnologData(PSS_RX)-128 ); //������ת��  ������ת
	printf("swerve  %d  \n\t",swerve);	
	if(swerve> 2)
	{
		swerve = (swerve-2)*100/126; //������ת 
	}
	else if(swerve< -2)
		{
			swerve = (swerve+2)*100/125; //������ת
		}
		 else swerve=0;		
}

//void key_mode_select(u8 keyput)
//{
//	switch(keyput)
//	{
//		case PSB_SELECT:    
//			break;
//		case PSB_L3:
//			break;
//		case PSB_R3:
//			break;
//		case PSB_START:    
//			break;
//		case PSB_PAD_UP:    
//			break;
//		case PSB_PAD_RIGHT:    
//			break;
//		case PSB_PAD_DOWN:    
//			break;
//		case PSB_PAD_LEFT:    
//			break;
//		case PSB_L2:    
//			break;
//		case PSB_R2:    
//			break;
//		case PSB_L1:    
//			break;
//		case PSB_R1:    
//			break;
//		case PSB_GREEN:    
//			break;
//		case PSB_RED:    
//			break;
//		case PSB_BLUE:    
//			break;
//		case PSB_PINK:    
//			break;
//		case PSB_TRIANGLE:    
//			break;
//		case PSB_CIRCLE:    
//			break;
//		case PSB_CROSS:    
//			break;
//		case PSB_SQUARE:    
//			break;
//		default: break;
//	}
//}