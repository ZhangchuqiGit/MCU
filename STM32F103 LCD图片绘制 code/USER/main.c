#include "sys.h"
#include "usart.h"
#include "delay.h"

#include "led.h"
#include "key.h"
#include "lcd.h"

//#include "picture.h"
//#include "font.h" 

//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}




int main(void)
 {	 
 	u8 x=0;
	u8 lcd_id[12];			//存放LCD ID字符串
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
		delay_ms(1000);	
  	while(1) 
	{		 
		delay_ms(100);	
LCD_Clear(WHITE); //清屏
gui_draw_hline(50,50,60,BRED);//画水平线
		delay_ms(100);	
LCD_Clear(WHITE); //清屏
gui_fill_circle(150,150,100,BRED);//画实心圆
		delay_ms(100);	
LCD_Clear(WHITE); //清屏
lcd_draw_bline(100,100,100,200,20,BRED);//画一条粗线
		
LCD_Draw_Circle(150,150,100);
		delay_ms(100);	
LCD_Clear(BROWN); //清屏
			delay_ms(100);	
	
		
		Picture_Draw(0,0,(u8 *) gImage_5  );//在指定座标范围显示一副图片
		delay_ms(1000);	
LCD_Clear(YELLOW); //清屏
		
//		switch(x)
//		{
//			case 0:LCD_Clear(WHITE);break;
//			case 1:LCD_Clear(BLACK);break;
//			case 2:LCD_Clear(BLUE);break;
//			case 3:LCD_Clear(RED);break;
//			case 4:LCD_Clear(MAGENTA);break;
//			case 5:LCD_Clear(GREEN);break;
//			case 6:LCD_Clear(CYAN);break;

//			case 7:LCD_Clear(YELLOW);break;
//			case 8:LCD_Clear(BRRED);break;
//			case 9:LCD_Clear(GRAY);break;
//			case 10:LCD_Clear(LGRAY);break;
//			case 11:LCD_Clear(BROWN);break;
//		}
//		POINT_COLOR=RED;	  
//		LCD_ShowString(30,40,210,24,24,"WarShip STM32 ^_^"); 
//		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
//		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
// 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID
//		LCD_ShowString(30,130,200,12,12,"2014/5/4");	      	
//	    x++;
//		if(x==12)x=0;
//		LED0=!LED0;				   		 
//		delay_ms(1000);	
	}
}
