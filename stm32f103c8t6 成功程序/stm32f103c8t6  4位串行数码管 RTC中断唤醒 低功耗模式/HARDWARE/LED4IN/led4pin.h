#ifndef LED4PIN_H
#define LED4PIN_H

#include "stm32f10x.h"
#include "sys.h"

#define led4pin_GPIO	 RCC_APB2Periph_GPIOA
#define led4pin_GPIOx	 GPIOA /* PAout */

#define	SCLK 		PAout(0)
#define	SCLK_pin 	GPIO_Pin_0

#define	RCLK 		PAout(1) 
#define	RCLK_pin 	GPIO_Pin_1

#define	DIO 		PAout(2) 
#define	DIO_pin  	GPIO_Pin_2

//extern u8 ds18B20_0;


void led4pin_Init(void);

/*                        hgfe dcba 
       __a__         0xC0=1100 0000 
     f|__g__|b            1111 1111 
     e|_____|c        
         d      * h   
		             
u8 LED_0F[sel_num] = 
{   0	 1	  2	   3	4	 5	  6	   7	8	 9  
   0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
   P/10 -/11 C/12 d/13 E/14 нч/15 -/16 ./17  
   0x8C,0xBF,0xC6,0xA1,0x86,0xFF, 0xbf,0x7f   };           */
void led4pin_Display(u8 LED_outbin,u8 sel_num);
void led4pin_go(u8 x);


#endif	
