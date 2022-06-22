
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
u16 my_abs(u16 x1,u16 x2);//����ֵ��|x1-x2|
void NRF_01(u8 num_nrf); //  1:RXģʽ      ;    0:TXģʽ  

static u8 flag_led_p=0;
u8 TX_RX=0,tmp_buf[20];
/*------------------------------------------------------*/
int main(void)
{	 
	u8 i=0;
	u8 tt=0;
	SystemInit();//ϵͳʱ�ӳ�ʼ�� 
//	Stm32_Clock_Init(9);//ϵͳʱ������ 
	JTAG_Set(SWD_ENABLE);
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	LED_Init(); //LED�˿ڳ�ʼ�� LED1,LED2
#if  ooioio
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
#else 
	NRF24L01_Init();    		//��ʼ��NRF24L01 
	while(NRF24L01_Check())
	{		
		TX_RX++;
		LED1=!LED1;
		delay_ms(100);
		if(TX_RX>=50)break;
	}
#endif
	SHT2X_Init();/*ʪ��,�¶�*/	
	ADC1_Init();
	LED1=1;
	LED2=1;
	TX_RX=2;//TXģʽ
	while(1)
	{
		tt++;
		if(tt>=253 && flag_led_p==0)
		{
			flag_led_p=1;
			TX_RX=2;//TXģʽ  
		}
		else if( flag_led_p==1)
			{
				flag_led_p=0;
				TX_RX=1;//RXģʽ  
			}
#if  ooioio
		TX_();
#else
		if(TX_RX==1)//RXģʽ  
		{
			NRF24L01_RX_Mode();//RXģʽ  
			TX_RX=3;
		}		    		    				 
		if(TX_RX==2)//TXģʽ  
		{
			NRF24L01_TX_Mode();//TXģʽ  
			TX_RX=4;			
		}	
		if(TX_RX==3)RX_();//RXģʽ
		if(TX_RX==4)      //TXģʽ  
		{
			TX_();//TXģʽ
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
/************** TXģʽ *****************/
int ADC_value[7]={1111,2222,3333,4444,0000,0000,0000};		
u8 tx_buf[4*7]="1999299939994999599969997999";  
void TX_(void)  
{
	u8 j=0;
 	u16 temperture=0;
    ADC_value[0]  = ADC_ConvertedValue[0]/4096.0*1000.0;//MQ135 ���� ���� 10-1000ppm 
    ADC_value[1]  = ADC_ConvertedValue[1]/4096.0*10000.0;//MQ2 ��ȼ������ 100~10000ppm
	ADC_value[2]  = ADC_ConvertedValue[2]/4096.0*1000.0;//MQ6   10~10000ppm Һ�������춡�顢����C3H8��LPG �����顢����C4H10��Һ��ʯ������
    ADC_value[3]  = ADC_ConvertedValue[3]/4096.0*1000.0;//MQ7 CO��10~1000ppm
//    ADC_value[4]  = ADC_ConvertedValue[4];//оƬ�ڲ��¶�ֵ 
	temperture=ADC_ConvertedValue[4]*(3300.0 /4096.0);		//��ѹֵ 
	temperture=100*((14300.0-temperture)/43.0+25); //����100��,ת��Ϊ�¶�ֵ 
	ADC_value[4]=temperture%10000/10;
	SHT2x_Calc_T();    /*�¶Ȳ���*/ // float temperatureC		C  
	ADC_value[5]=temperatureC*100;
	SHT2x_Calc_RH();   /*ʪ�Ȳ���*/ // float humidityRH  		%
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
/************** RXģʽ *****************/
void RX_(void)  
{
	if( NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
	{
		if(tmp_buf[0]=='+')LED1=0;
		else LED1=1;
	}
	else {}
}
#if  ooioio
//������֮��ľ���ֵ 
//x1,x2����ȡ��ֵ��������
//����ֵ��|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  

void NRF_01(u8 num_nrf) //  1:RXģʽ      ;    0:TXģʽ  
{
	u8 i=0;
	NRF24L01_Init();    		//��ʼ��NRF24L01 
	while(NRF24L01_Check())
	{		
		i++;
		if(i>=250)break;
	}	
	if(num_nrf)NRF24L01_RX_Mode();//RXģʽ  
	else NRF24L01_TX_Mode();//TXģʽ  
}
#endif
