
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
//#include "key.h"
#include "adc.h"
#include "24l01.h" 	 
#include "lcd.h"
#include "myiic.h"
#include "SHT2X.h"

#define ooioio   0  

void str_char_p(int ssdata,u8 *p,u8 i);
void RX_(void);
void TX_(void);
u16 my_abs(u16 x1,u16 x2);//返回值：|x1-x2|
void NRF_01(u8 num_nrf); //  1:RX模式      ;    0:TX模式  

static u8 flag_led_p=0;
u8 TX_RX=0,tmp_buf[20];
/*------------------------------------------------------*/
int main(void)
{	 
	u8 i=0;
	u8 tt=0;
	SystemInit();//系统时钟初始化 
//	Stm32_Clock_Init(9);//系统时钟设置 
	JTAG_Set(SWD_ENABLE);
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	LED_Init(); //LED端口初始化 LED1,LED2
#if  ooioio
	uart_init(115200);	 	//串口初始化为115200
#else 
	NRF24L01_Init();    		//初始化NRF24L01 
	while(NRF24L01_Check())
	{		
		TX_RX++;
		LED1=!LED1;
		delay_ms(100);
		if(TX_RX>=50)break;
	}
#endif
	SHT2X_Init();/*湿度,温度*/	
	ADC1_Init();
	LED1=1;
	LED2=1;
	TX_RX=2;//TX模式
	while(1)
	{
		tt++;
		if(tt>=253 && flag_led_p==0)
		{
			flag_led_p=1;
			TX_RX=2;//TX模式  
		}
		else if( flag_led_p==1)
			{
				flag_led_p=0;
				TX_RX=1;//RX模式  
			}
#if  ooioio
		TX_();
#else
		if(TX_RX==1)//RX模式  
		{
			NRF24L01_RX_Mode();//RX模式  
			TX_RX=3;
		}		    		    				 
		if(TX_RX==2)//TX模式  
		{
			NRF24L01_TX_Mode();//TX模式  
			TX_RX=4;			
		}	
		if(TX_RX==3)RX_();//RX模式
		if(TX_RX==4)      //TX模式  
		{
			TX_();//TX模式
			i++;
			if(i>30) i=0;	
			if(i<5)
			{
				LED2=0;
			}
			else LED2=1;	
		}
#endif
	}
}
/************** TX模式 *****************/
int ADC_value[7]={1111,2222,3333,4444,0000,0000,0000};		
u8 tx_buf[4*7]="1999299939994999599969997999";  
void TX_(void)  
{
	u8 j=0;
 	u16 temperture=0;
    ADC_value[0]  = ADC_ConvertedValue[0]/4096.0*1000.0;//MQ135 氨气 烟雾 10-1000ppm 
    ADC_value[1]  = ADC_ConvertedValue[1]/4096.0*10000.0;//MQ2 可燃性气体 100~10000ppm
	ADC_value[2]  = ADC_ConvertedValue[2]/4096.0*1000.0;//MQ6   10~10000ppm 液化气、异丁烷、丙烷C3H8、LPG （丙烷、丁烷C4H10、液化石油气）
    ADC_value[3]  = ADC_ConvertedValue[3]/4096.0*1000.0;//MQ7 CO：10~1000ppm
//    ADC_value[4]  = ADC_ConvertedValue[4];//芯片内部温度值 
	temperture=ADC_ConvertedValue[4]*(3300.0 /4096.0);		//电压值 
	temperture=100*((14300.0-temperture)/43.0+25); //扩大100倍,转换为温度值 
	ADC_value[4]=temperture%10000/10;
	SHT2x_Calc_T();    /*温度测试*/ // float temperatureC		C  
	ADC_value[5]=temperatureC*100;
	SHT2x_Calc_RH();   /*湿度测试*/ // float humidityRH  		%
	ADC_value[6]=humidityRH*100;
#if !ooioio
	for(j=0;j<7;j++)
	{
		str_char_p(ADC_value[j],tx_buf,j);
	}
	if( NRF24L01_TxPacket( tx_buf )==TX_OK ){}
	else {}
#endif
#if  ooioio
		for(j=0;j<7;j++)printf("%d:%d  ",j,ADC_value[j]);//  ppm
	printf("\r\n");
//	delay_ms(200);
#endif
}
/*---------------------------------------------*/
void str_char_p(int ssdata,u8 *p,u8 j)
{
	u8 i=4*j;
	p[i]=  (u8)(ssdata/1000%10+0x30);
	p[i+1]=(u8)(ssdata/100%10+0x30);
	p[i+2]=(u8)(ssdata/10%10+0x30);
	p[i+3]=(u8)(ssdata%10+0x30);
}
/************** RX模式 *****************/
void RX_(void)  
{
	if( NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
	{
		if(tmp_buf[0]=='+')LED1=0;
		else LED1=1;
	}
	else {}
}
#if  ooioio
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  

void NRF_01(u8 num_nrf) //  1:RX模式      ;    0:TX模式  
{
	u8 i=0;
	NRF24L01_Init();    		//初始化NRF24L01 
	while(NRF24L01_Check())
	{		
		i++;
		if(i>=250)break;
	}	
	if(num_nrf)NRF24L01_RX_Mode();//RX模式  
	else NRF24L01_TX_Mode();//TX模式  
}
#endif
