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
s16 speed =0 , swerve/*转弯量*/=0 ;

/*****************************************/
void speed_select(void);//得到一个摇杆的模拟量  范围0~256 
void stop_fast(void);
void stop_init(void); 

/*********         ***********      main        ***********             *********/
int main(void)
{
	SystemInit();//系统时钟设置
	delay_init();	     //延时初始化
	uart_init(115200);  //串口1初始化 
/*  中断/溢出  时间 Tout us=   250 * 72 / 72 = 250 us  */
	TIM3_PWM_Init(speed_max-1,72-1);
	LED_Init();
	PS2_Init();			 //驱动端口初始化
	PS2_SetInit();		 //配配置初始化,配置“红绿灯模式”，并选择是否可以修改 //开启震动模式
	M_Init();	   //电机旋转方向控制信号端口初始化	
    stop_init( );
	while(1)
	{	
		if(flag_delay1>=110)/*11ms*/
		{	
			flag_delay1=0;
			if( PS2_RedLight() ==0 )//返回值0-红灯模式
			{			
				delay_ms(4); /*4ms!!!!!*/ 
				LED = 0;//判断手柄是否为红灯模式，是，指示灯LED点亮
				flywheel = PS2_DataKey(); //手柄按键捕获处理 
				speed_select();//得到一个摇杆的模拟量  范围0~256 				
				Motor__Control(speed,swerve/*转弯量*/);//电机  控制  //speed:前进/后退   swerve:左转/右转 
				switch(flywheel)
				{
					case PSB_L1: F_1 = 0;F_2 = 1;break;
					case PSB_L2: F_2 = 0;break;
//					case PSB_L3: PS2_Vibration(0xbf,0x00);/*发出震动后必须有延时delay_ms(1000)*/;break;
					case PSB_R1: F_2 = 0;F_1 = 1;break;
					case PSB_R2: F_1 = 0;break;
//					case PSB_R3: PS2_Vibration(0x00,0xbf);/*发出震动后必须有延时delay_ms(1000)*/;break;
					default: break;
				}
			}
			else	//判断手柄不是红灯模式
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
		delay_us(100);//计时
	}	 
}

/**************        *****************  得到一个摇杆的模拟量  范围0~256  *****************          *****************/
void speed_select(void)//得到一个摇杆的模拟量  范围0~256 
{	 
//	printf("speed_select\r\n");
	speed = -( PS2_AnologData(PSS_LY)-127 ); //正：后退；  负：前进
//	printf("范围0~125  %d\t",speed);
	if(flywheel == PSB_L3)
	{	/* 加速  125*80/35=285  */
		if(speed> 2)speed = (speed-2)*80/35  ; //正：后退；
		else if(speed< -2)speed = (speed+2)*80/35 ; //负：前进
			 else speed=0; 
	}
	else 
	{	/* 慢速  125*20/33= 75 */
		if(speed> 2)speed = (speed-2)*20/33; //正：后退；
		else if(speed< -2)speed = (speed+2)*20/33; //负：前进
			 else speed=0;
	}
//	printf("speed  %d\t",speed);
	swerve = -( PS2_AnologData(PSS_RX)-128 ); //正：右转；  负：左转
//	printf("范围0~126  %d\t",swerve);
	if(flywheel == PSB_R3)
	{	/* 加速 125*40/19=263 */
		if(swerve> 2)swerve = (swerve-2)*40/19 ;  //正：右转 
		else if(swerve< -2)swerve = (swerve+2)*40/19 ; //负：左转
			 else swerve=0;	
	}
	else
	{	/* 慢速  125*20/48=52 */
		if(swerve> 2)swerve = (swerve-2)*20/48; //正：右转 
		else if(swerve< -2)swerve = (swerve+2)*20/48; //负：左转
			 else swerve=0;	
	}
//	speed = -(PS2_AnologData(PSS_LY)-127)*7;	//正：前进；  负：后退
//	swerve = -(PS2_AnologData(PSS_RX)-127)*7;	//正：左转；  负：右转
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

