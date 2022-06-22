#include "led4pin.h"

//u8 ds18B20_0=0;

void led4pin_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;	
	
 	RCC_APB2PeriphClockCmd(led4pin_GPIO, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = SCLK_pin|RCLK_pin|DIO_pin;			
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(led4pin_GPIOx, &GPIO_InitStructure);				
	
 	GPIO_ResetBits(led4pin_GPIOx,SCLK_pin|RCLK_pin|DIO_pin);
}

void led4pin_Display(u8 LED_outbin,u8 sel_num)
{
/*                        hgfe dcba 
       __a__         0xC0=1100 0000 
     f|__g__|b            1111 1111 
     e|_____|c        
         d      * h   
		             */
	u8 LED_0F[] = 
	{/* 0	 1	  2	   3	4	 5	  6	   7	8	 9  */
	   0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
	/* P/10 -/11 C/12 d/13 E/14 ��/15 _/16 ./17  */	
	   0x8C,0xBF,0xC6,0xA1,0x86,0xFF, 0xf7,0x7f   };
	u8 point=0,LED_num=LED_0F[sel_num];

	switch(LED_outbin)
	{
		case 1: point=0x08;//��ʾ��1λ
			break;
		case 2: point=0x04;//��ʾ��2λ
			break;
		case 3: point=0x02;//��ʾ��3λ	
				LED_num = LED_num+0x80;/* DS18B20 ��С���� */
//				if(ds18B20_0==1)LED_num = LED_num+0x80;/* DS18B20 ��С���� */
			break;
		case 4: point=0x01;//��ʾ��4λ
			break;
		default: 
			break;
	}
	led4pin_go(LED_num);
	led4pin_go(point);
	RCLK=0;
	RCLK=1;
}
void led4pin_go(u8 x)
{
	u8 i;
	for(i=8;i>=1;i--)
	{
		if(x&0x80) DIO=1;//��
		else DIO=0;//��	
		x<<=1;
		SCLK=0;//ִ��go
		SCLK=1;//ִ��ok
	}    
}
