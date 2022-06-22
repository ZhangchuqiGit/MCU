#include<reg51_typedef.h>
/***++++++++++++++++++++****/
#include"lcd.h"
#include"ds1302.h"
#include"keyscan.h"
#include<lcddisplay.h>
#include"timer.h"

void time_exchange(void);
void menu0_class(void); //功能选择与处理 0    菜单
void menu1_select(void);	//功能选择与处理 1  时间设置
void menu2_class(void);	//功能选择与处理 2  课表时间查看
void menu3_class(void); //功能选择与处理 3   课表时间调整

void delay1s(void)   //误差 0us
{
    u8 a,b,c;
    for(c=46;c>0;c--)
        for(b=152;b>0;b--)
            for(a=70;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}
/******************* main ***********************/
void main()
{
	u8 Timer0_class_on=0;
	beep=0;
	Tim_on=0;
	Tim_on1=0;
	LcdInit();
	LcdWriteCom(0x80);//定光标
	LcdWriteCom(0x01);  //清屏
	Lcd_Str(0,0,"#_#   zcq  *^-^*");		
	Lcd_Str(0,1,"   16034470242  ");		
	delay1s();
	Ds1302Init();
	Timer0_Init(1,500);//beep
	while(1)
	{	
		Ds1302ReadTime();
		LcdDisplay();
		if(KEYx_Scan(0,1)==1)menu0_class();
		if(KEYx_Scan(0,4)==4)Tim_on1=!Tim_on1;
		sec=TIME[6]/16*10+TIME[6]%16;
		if(sec==0)
		{
			hour=(TIME[4]/16)*10+TIME[4]%16;
			min=(TIME[5]/16)*10+TIME[5]%16;
			if(     ( hour==time_class_s[0][0] && min==time_class_s[0][1] )|| /*  8,00, */
					( hour==time_class_s[0][2] && min==time_class_s[0][3] )|| /*  8,45, */
					( hour==time_class_s[0][4] && min==time_class_s[0][5] )|| /*  8,55, */
					( hour==time_class_s[0][6] && min==time_class_s[0][7] )|| /*  9,40  */
				
					( hour==time_class_s[1][0] && min==time_class_s[1][1] )|| /*  10,00, */
					( hour==time_class_s[1][2] && min==time_class_s[1][3] )|| /*  10,45, */
					( hour==time_class_s[1][4] && min==time_class_s[1][5] )|| /*  10,55, */
					( hour==time_class_s[1][6] && min==time_class_s[1][7] )|| /*  11,40, */
				
					( hour==time_class_s[2][0] && min==time_class_s[2][1] )|| /*  14,30, */
					( hour==time_class_s[2][2] && min==time_class_s[2][3] )|| /*  15,15, */
					( hour==time_class_s[2][4] && min==time_class_s[2][5] )|| /*  15,25, */
					( hour==time_class_s[2][6] && min==time_class_s[2][7] )|| /*  16,10, */
				
					( hour==time_class_s[3][0] && min==time_class_s[3][1] )|| /*  16,20, */
					( hour==time_class_s[3][2] && min==time_class_s[3][3] )|| /*  17,50, */
					( hour==time_class_s[3][4] && min==time_class_s[3][5] )|| /*  16,20, */
					( hour==time_class_s[3][6] && min==time_class_s[3][7] )|| /*  17,50, */
				
					( hour==time_class_s[4][0] && min==time_class_s[4][1] )|| /*  19,40, */
					( hour==time_class_s[4][2] && min==time_class_s[4][3] )|| /*  20,15, */
					( hour==time_class_s[4][4] && min==time_class_s[4][5] )|| /*  20,25, */
					( hour==time_class_s[4][6] && min==time_class_s[4][7] )   /*  21,20  */      )
			{
				Timer0_class_on=1;
				Tim_on=1;
			}
		}
		if(Timer0_class_on==1)
		{
			if(sec>=40)//5s
			{	
				Tim_on=0;
				beep=0;				
				Timer0_class_on=0;
			}
			else if(sec>=35)//30s
			{	
				Tim_on=1;
			}	
			else if(sec>=5 )//5s
			{	
				Tim_on=0;
				beep=0;		
			}	
		}
	}
}	
/****---------------------------------------------****/
void time_exchange(void)
{
	year=TIME[0]/16*10+TIME[0]%16;
	month=TIME[1]/16*10+TIME[1]%16;
	date=TIME[2]/16*10+TIME[2]%16;
	week=TIME[3]&0x07;
	hour=TIME[4]/16*10+TIME[4]%16;
	min=TIME[5]/16*10+TIME[5]%16;
	sec=TIME[6]/16*10+TIME[6]%16;
}
/****---------------------------------------------****/
void menu1_select(void)	//功能选择与处理 1  时间设置
{
	u8 num=0,i=0;
	LcdDisplay();
	time_exchange();
	while(1)
	{			
		LcdWriteCom(0x80+num_buf[num]);//定光标	
		if(KEYx_Scan(0,2)==2)
		{
			num++;
			if(num>=6)num=0;
		}
		if(KEYx_Scan(1,3)==3)
		{
			switch(num_buf[num])
			{
				case 1: hour++;if(hour>=24)hour=0;TIME[4]=hour/10*16+hour%10;				
						Lcd_ASCII(0,0,'0'+hour/10);
						Lcd_ASCII(1,0,'0'+hour%10);
				break;
				case 4:	min++;if(min>=60)min=0;TIME[5]=min/10*16+min%10;					
						Lcd_ASCII(3,0,'0'+min/10);
						Lcd_ASCII(4,0,'0'+min%10);
				break;
				case 7:	sec++;if(sec>=60)sec=0;TIME[6]=sec/10*16+sec%10;					
						Lcd_ASCII(6,0,'0'+sec/10);
						Lcd_ASCII(7,0,'0'+sec%10);
				break;
				case 0x40+3: year++;if(year>=100)year=0;TIME[0]=year/10*16+year%10;				
						Lcd_ASCII(2,1,'0'+year/10);
						Lcd_ASCII(3,1,'0'+year%10);
				break;
				case 0x40+6: month++;if(month>12)month=1;TIME[1]=month/10*16+month%10;				
						Lcd_ASCII(5,1,'0'+month/10);
						Lcd_ASCII(6,1,'0'+month%10);
						if(month==2)              
						{
							if( !leap_year()) //非闰年的条件   
							{
								if(date==29)date=1; //自动校正天数   
							}
						}
				break;
				case 0x40+9: date++;			
						if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)if (date>31) date=1;//大月31天   
						if(month==4||month==6||month==9||month==11)if (date>30) date=1;//小月30天   
						if(month==2)              
						{
							if( leap_year( )) //闰年的条件   
							{
								if(date>29)date=1; //闰年2月为29天   
							}
							else   //平年2月为28天   
							{
								if(date>28)date=1;
							}
						}
						TIME[2]=date/10*16+date%10;		
						Lcd_ASCII(8,1,'0'+date/10);
						Lcd_ASCII(9,1,'0'+date%10);
				break;
			}
			week=week_proc( ) ;//星期自动运算 
			if( (TIME[3]-week)!=0 )
			{			
				Lcd_ASCII(13,1, EN_week[week][0]);
				Lcd_ASCII(14,1, EN_week[week][1]);
				Lcd_ASCII(15,1, EN_week[week][2]);
			}
			TIME[3]=week&0x07;	
		}
		if(KEYx_Scan(0,1)==1)
		{
			Ds1302Init();
			Lcd_SetState_0( );
			break;
		}
	}
}
/****---------------------------------------------****/
void menu2_class(void)	//功能选择与处理 2  课表时间查看
{
	u8 num=0,if_num=0;
	LcdWriteCom(0x0c)/*开显示,不显示光标*/; 
	LcdWriteCom(0x80);//定光标
	LcdWriteCom(0x01);  //清屏
	dis_class(num);
	while(1)
	{
		if(KEYx_Scan(0,2)==2)
		{
			num++;
			if(num>=5)num=0;
		}	
		if( (num-if_num) !=0 )
		{
			if_num=num;
			dis_class(num);
		}

		if(KEYx_Scan(0,1)==1)
		{
			Lcd_SetState_0( );
			break;
		}
	}
}
/****---------------------------------------------****/
void menu3_class(void) //功能选择与处理 3   课表时间调整
{
	u8 num=0,g_num=0,t=0,if_num=0;
	LcdWriteCom(0x80);//定光标
	LcdWriteCom(0x01);  //清屏
	dis_class(0);
	LcdWriteCom(time_class_i[num]);//定光标
	while(1)
	{
		if(KEYx_Scan(0,2)==2)
		{
			num++;
			if(num>=8)
			{
				num=0;
				g_num++;
				if(g_num>=5)g_num=0;
			}
			LcdWriteCom(time_class_i[num]);//定光标
		}
		if( (g_num-if_num) !=0 )
		{
			if_num=g_num;
			dis_class(g_num);
			LcdWriteCom(time_class_i[num]);//定光标
		}
		if(KEYx_Scan(1,3)==3)
		{
			switch(num)
			{
				case 0: time_class_s[g_num][num]++; 
						if( time_class_s[g_num][num] >= 24 )time_class_s[g_num][num]=0;
						Lcd_ASCII(5,0,'0'+time_class_s[g_num][num]/10);
						Lcd_ASCII(6,0,'0'+time_class_s[g_num][num]%10);
						LcdWriteCom(0x80+6);//定光标
				break;
				case 1:	time_class_s[g_num][num]++;
						if( time_class_s[g_num][num] >= 60 )time_class_s[g_num][num]=0;
						Lcd_ASCII(8,0,'0'+time_class_s[g_num][num]/10);
						Lcd_ASCII(9,0,'0'+time_class_s[g_num][num]%10);
						LcdWriteCom(0x80+9);//定光标
				break;
				case 2:	time_class_s[g_num][num]++;
						if( time_class_s[g_num][num] >= 24 )time_class_s[g_num][num]=0;
						Lcd_ASCII(11,0,'0'+time_class_s[g_num][num]/10);
						Lcd_ASCII(12,0,'0'+time_class_s[g_num][num]%10);
						LcdWriteCom(0x80+12);//定光标
				break;
				case 3: time_class_s[g_num][num]++;
						if( time_class_s[g_num][num] >= 60 )time_class_s[g_num][num]=0;
						Lcd_ASCII(14,0,'0'+time_class_s[g_num][num]/10);
						Lcd_ASCII(15,0,'0'+time_class_s[g_num][num]%10);
						LcdWriteCom(0x80+15);//定光标
				break;
				case 4: time_class_s[g_num][num]++; 
						if( time_class_s[g_num][num] >= 24 )time_class_s[g_num][num]=0;
						Lcd_ASCII(5,1,'0'+time_class_s[g_num][num]/10);
						Lcd_ASCII(6,1,'0'+time_class_s[g_num][num]%10);
						LcdWriteCom(0x80+0x40+6);//定光标
				break;
				case 5:	time_class_s[g_num][num]++;
						if( time_class_s[g_num][num] >= 60 )time_class_s[g_num][num]=0;
						Lcd_ASCII(8,1,'0'+time_class_s[g_num][num]/10);
						Lcd_ASCII(9,1,'0'+time_class_s[g_num][num]%10);
						LcdWriteCom(0x80+0x40+9);//定光标
				break;
				case 6:	time_class_s[g_num][num]++;
						if( time_class_s[g_num][num] >= 24 )time_class_s[g_num][num]=0;
						Lcd_ASCII(11,1,'0'+time_class_s[g_num][num]/10);
						Lcd_ASCII(12,1,'0'+time_class_s[g_num][num]%10);
						LcdWriteCom(0x80+0x40+12);//定光标
				break;
				case 7: time_class_s[g_num][num]++;
						if( time_class_s[g_num][num] >= 60 )time_class_s[g_num][num]=0;
						Lcd_ASCII(14,1,'0'+time_class_s[g_num][num]/10);
						Lcd_ASCII(15,1,'0'+time_class_s[g_num][num]%10);
						LcdWriteCom(0x80+0x40+15);//定光标
				break;
			}
			LcdWriteCom(time_class_i[num]);//定光标
		}
		if(KEYx_Scan(0,1)==1)
		{
			Lcd_SetState_0( );
			break;
		}
	}
}
/****---------------------------------------------****/
void menu0_class(void) //功能选择与处理 0    菜单
{
	u8 num=0;
	LcdWriteCom(0x80);//定光标
	LcdWriteCom(0x01);  //清屏
	Lcd_Str(0,0,"time*set     zcq");		
	Lcd_Str(0,1,"*class     *just");		
	LcdWriteCom(0x0f)/*光标闪烁*/; 
	while(1)
	{
		if(KEYx_Scan(0,2)==2)
		{
			num++;
			if(num>=3)num=0;
		}	
		if(num==0)
		{
			LcdWriteCom(0x84);//定光标	
			if(KEYx_Scan(0,3)==3)
			{
				menu1_select( );	//功能选择与处理 1		
				break;
			}
		}
		else if(num==1)
			{
				LcdWriteCom(0x80+0x40+0);//定光标
				if(KEYx_Scan(0,3)==3)
				{					
					menu2_class( );	//功能选择与处理 2		
					break;
				}
			}
			else if(num==2)
				{
					LcdWriteCom(0x80+0x40+11);//定光标
					if(KEYx_Scan(0,3)==3)
					{					
						menu3_class( );	//功能选择与处理 3		
						break;
					}
				}
		if(KEYx_Scan(0,1)==1)
		{
			Lcd_SetState_0( );
			break;
		}
	}
}
