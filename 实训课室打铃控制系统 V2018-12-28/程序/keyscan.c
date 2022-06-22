#include"keyscan.h"

void delay12ms(void)   //误差 0us
{
    u8 a,b;
    for(b=129;b>0;b--)
        for(a=45;a>0;a--);
}
u8 KEYx_Scan(u8 mode,u8 Kx)//mode==1  支持连按		  
{	 
	static u8 key_up1=1;//按键按松开标志
	static u8 key_up2=1;//按键按松开标志
	static u8 key_up3=1;//按键按松开标志
	static u8 key_up4=1;//按键按松开标志
	if( K1==0 || K2==0 || K3==0 || K4==0)
	{
		switch(Kx)
		{
			case 1:
				if(mode)key_up1=1;  //mode==1  支持连按		  
				if(key_up1 && K1==0)
				{
					delay12ms();//去抖动 
					key_up1=0;
					if(K1==0)return Kx;
				}else if(K1==1)key_up1=1; 	    
				return 0;// 无按键按下
			break;
			case 2:
				if(mode)key_up2=1;  //mode==1  支持连按		  
				if(key_up2 && K2==0)
				{
					delay12ms();//去抖动 
					key_up2=0;
					if(K2==0)return Kx;
				}else if(K2==1)key_up2=1; 	    
				return 0;// 无按键按下
			break;
			case 3:
				if(mode)key_up3=1;  //mode==1  支持连按		  
				if(key_up3 && K3==0)
				{
					delay12ms();//去抖动 
					key_up3=0;
					if(K3==0)return Kx;
				}else if(K3==1)key_up3=1; 	    
				return 0;// 无按键按下
			break;
			case 4:
				if(mode)key_up4=1;  //mode==1  支持连按		  
				if(key_up4 && K4==0)
				{
					delay12ms();//去抖动 
					key_up4=0;
					if(K4==0)return Kx;
				}else if(K4==1)key_up4=1; 	    
				return 0;// 无按键按下
			break;
		}	
	}
}
