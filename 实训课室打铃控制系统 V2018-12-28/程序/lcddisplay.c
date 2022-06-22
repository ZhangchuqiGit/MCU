#include<lcddisplay.h>

u8 armhour=0,armmin=0,armsec=0;//闹钟时、分、秒   
u8 year=0,month=0,date=0,week=0;//年、月、日、星期   
u8 hour=0,min=0,sec=0;//时、分、秒   
u8 code num_buf[6]= /*光标移动*/ 
	{ 1,4,7,  	0x40+3,0x40+6,0x40+9};//lcd显示缓冲区   
u8 code Alarm_clock_buf[4]= /*光标移动*/ 
	{ 1, 4, 7      ,0x40+12 };//lcd显示缓冲区   
char code EN_week[][3]={"SUN","MON","TUE","WED","THU","FRI","SAT","SUN"}; 
u8 Alarm_clock[]={0,0,58,0,0,58/*  ASCII :=58 */,0,0};//"00:00:00"
u8 code para_month[13]={0,0,3,3,6,1,4,6,2,5,0,3,5};  //星期月参变数
u8 time_class_s[5][8]={ { 8,00,8,45,	 8,55, 9,40},/*12*/
						{10,00,10,45,   10,55,11,40},/*34*/
						{14,30,15,15,   15,25,16,10},/*56*/	
						{16,20,17,50, 	16,20,17,50},/*78*/
						{19,40,20,15,	20,25,21,20} };
u8 time_class_i[8]={ 0x86, 0x89, 0x8c, 0x8f, 0x40+0x86, 0x40+0x89, 0x40+0x8c, 0x40+0x8f };

//闰年的计算   
u8 leap_year( void )   
{   
    u8 leap;   
    if( (year%4==0&&year%100!=0)||(year%400==0) )//闰年的条件   
        leap=1;   
    else   
        leap=0;  
    return leap;   
}  
//星期的自动运算和处理   
u8 week_proc(void)   
{   
	u8 c,num_leap,week;    
    num_leap=year/4-year/100+year/400;//自00年起到year所经历的闰年数   
    if( leap_year( ) && month<=2 )   //既是闰年且是1月和2月    
        c=5;   
    else    
        c=6;   
    week=(year+para_month[month]+date+num_leap+c)%7;//计算对应的星期   
    return week;   
} 
//主界面
void LcdDisplay(void)
{
	u8 week=0;
	LcdWriteCom(0x80); //设置数据指针起点	
	LcdWriteData('0'+TIME[4]/16); 
    LcdWriteData('0'+TIME[4]%16);    
    LcdWriteData(':'); //58  ASCII
    LcdWriteData('0'+TIME[5]/16);
    LcdWriteData('0'+TIME[5]%16);    
    LcdWriteData(':'); //58  ASCII
    LcdWriteData('0'+TIME[6]/16);
    LcdWriteData('0'+TIME[6]%16);   
    LcdWriteData(' ');   
    LcdWriteData(' ');   
    LcdWriteData('z');   
    LcdWriteData('c');  
    LcdWriteData('q');   
    LcdWriteData(' ');   
	LcdWriteCom(0x80+14); //设置数据指针起点	
	if(Tim_on1==1)LcdWriteData('!'); 
	else LcdWriteData(' '); 
	if(Tim_on==1)LcdWriteData('*');  
	else LcdWriteData(' '); 
	LcdWriteCom(0x80+0x40);//设置数据指针起点	
	LcdWriteData('2'); //'0'=48  ASCII
    LcdWriteData('0');     
    LcdWriteData('0'+TIME[0]/16);
    LcdWriteData('0'+TIME[0]%16);    
    LcdWriteData('-'); 
    LcdWriteData('0'+TIME[1]/16);
    LcdWriteData('0'+TIME[1]%16);    
    LcdWriteData('-'); 
    LcdWriteData('0'+TIME[2]/16);
    LcdWriteData('0'+TIME[2]%16);    
    LcdWriteData(' ');   
    LcdWriteData(' ');   
    LcdWriteData(' ');   
	LcdWriteCom(0x80+0x40+13);//设置数据指针起点	
	week=TIME[3]&0x07;
    LcdWriteData(EN_week[week][0]);    
    LcdWriteData(EN_week[week][1]);    
    LcdWriteData(EN_week[week][2]);    
}
//主界面显示设置
void Lcd_SetState_0(void)
{
	LcdWriteCom(0x0c)/*开显示,不显示光标*/; 
	LcdWriteCom(0x80);//定光标
	LcdWriteCom(0x01);  //清屏
//	LcdWriteCom(0x80+0x40);//定光标
//	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x06);//写一个光标,地址指针加1
//	LcdWriteCom(0x80);//定光标
	LcdDisplay();
}		
void dis_class(u8 NUM) // 课表时间查看
{
	u8 t=0;
	LcdWriteCom(0x80); //设置数据指针起点	
	t=NUM*2+1;
	LcdWriteData('N'); 
    LcdWriteData('O');    
    LcdWriteData('0'+t);    
    LcdWriteData(' ');   
    LcdWriteData(' ');   
    LcdWriteData('0'+time_class_s[NUM][0]/10);    
    LcdWriteData('0'+time_class_s[NUM][0]%10);    
	LcdWriteData(':'); //58  ASCII
    LcdWriteData('0'+time_class_s[NUM][1]/10);    
    LcdWriteData('0'+time_class_s[NUM][1]%10);    
    LcdWriteData('-'); 
    LcdWriteData('0'+time_class_s[NUM][2]/10);    
    LcdWriteData('0'+time_class_s[NUM][2]%10);    
	LcdWriteData(':'); //58  ASCII
    LcdWriteData('0'+time_class_s[NUM][3]/10);    
    LcdWriteData('0'+time_class_s[NUM][3]%10); 
	
	LcdWriteCom(0x80+0x40); //设置数据指针起点	
	LcdWriteData('N'); 
	LcdWriteData('O');    
	if(t==9)
	{
		Lcd_ASCII(2,1,'0'+1);   
		Lcd_ASCII(3,1,'0'+0);   
	}
	else 
	{
		Lcd_ASCII(2,1,'0'+t+1);   
		Lcd_ASCII(3,1,' ');   
	}
    LcdWriteData(' ');   
    LcdWriteData('0'+time_class_s[NUM][4]/10);    
    LcdWriteData('0'+time_class_s[NUM][4]%10);    
	LcdWriteData(':'); //58  ASCII
    LcdWriteData('0'+time_class_s[NUM][5]/10);    
    LcdWriteData('0'+time_class_s[NUM][5]%10);    
    LcdWriteData('-'); 
    LcdWriteData('0'+time_class_s[NUM][6]/10);    
    LcdWriteData('0'+time_class_s[NUM][6]%10);    
	LcdWriteData(':'); //58  ASCII
    LcdWriteData('0'+time_class_s[NUM][7]/10);    
    LcdWriteData('0'+time_class_s[NUM][7]%10);    
}

