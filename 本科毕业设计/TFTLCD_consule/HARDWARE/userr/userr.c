#include "userr.h"

u8 tx_buf[32]={0};  
//              0123/4567/8901/2345/6789/0123
u8 flag_func=0;
u8 NRF_Check=0;
u8 nrf_gg=1;
u8 TX_RX=0;
u8 nrf_d=0;
u8 bnb[2]={0};

#if  ooioio
void display_sensor(void)
{
	u8 i=0;
	LCD_Fill(0,0,479-65,319, LBBLUE); //填充单色
	Picture_Draw(479-65-39,0,(u8 *) gImage_3 );//在指定座标范围显示一副图片
	Picture_Draw(479-60,319-60,(u8 *) gImage_1 );//在指定座标范围显示一副图片
	BACK_COLOR=LBBLUE;  //背景色 
	POINT_COLOR=GREEN;//设置字体 画笔颜色
	LCD_ShowString_user(90,2,24,"Sensor detection");//显示一个字符串, 12/16/24字体	     
	lcd_draw_bline(20,35,479-65-20-40,35,2,BROWN);//画一条粗线
	gui_draw_hline(10,50,390,WHITE);//画水平线1
	gui_draw_hline(10,50+130,390,WHITE);//画水平线2
	gui_draw_hline(10,319-10,390,WHITE);//画水平线3
	POINT_COLOR=WHITE;//设置字体 画笔颜色
	LCD_DrawLine(10,50,10,319-10); //画线1
	LCD_DrawLine(10+130,50,10+130,319-10); //画线2	
	LCD_DrawLine(10+130*2,50,10+130*2,319-10); //画线3
	LCD_DrawLine(400,50,400,319-10); //画线4
	POINT_COLOR=RED;//设置字体 画笔颜色
	LCD_ShowString_user(10+5,51,24,"MQ135");//显示一个字符串, 12/16/24字体	     
	LCD_ShowString_user(10+130+5,51,24,"MQ2");//显示一个字符串, 12/16/24字体	     
	LCD_ShowString_user(10+130*2+5,51,24,"MQ6");//显示一个字符串, 12/16/24字体	     
	LCD_ShowString_user(15,50+130+2,24,"MQ7");//显示一个字符串, 12/16/24字体	     
	LCD_ShowString_user(10+130+5,50+130+2,24,"sys_temp");//显示一个字符串, 12/16/24字体	   			
	LCD_ShowString_user(10+130*2+5,50+130+2,24,"SHT20");//显示一个字符串, 12/16/24字体	   			

	for(i=0;i<7;i++)
	{
		switch(i)
		{
			case 0:	//MQ135 氨气 烟雾 10-1000ppm 		
				POINT_COLOR=BROWN;//设置字体 画笔颜色	
				LCD_ShowString_user(95,72,24,"ppm");//显示一个字符串, 12/16/24字体 
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString_user(95,55,16,"/1000");//显示一个字符串, 12/16/24字体 
				POINT_COLOR=BLACK;//设置字体 画笔颜色	
				LCD_ShowString(20,120,12*2,18,24,"NH");// NH3
				LCD_ShowNum(20+2+12*2,120+15,3,1,12); //显示一个数字		
				LCD_ShowChar_user(20+92,120,'H',24,1) ;//H2
				LCD_ShowNum(20+90+12+3,120+15,2,1,12); //显示一个数字		
				LCD_ShowNum(20+1+12*2,120+15,3,1,12); //显示一个数字		
				LCD_ShowChar_user(20,120+25,'C',24,1) ;// C6H5CH3
				LCD_ShowNum(20+12+1,120+25+15,6,1,12); //显示一个数字		
				LCD_ShowChar_user(20+18+2,120+25,'H',24,1) ;//显示一个字符
				LCD_ShowNum(20+18+4+12,120+25+15,5,1,12); //显示一个数字		
				LCD_ShowString(20+12*3+4,120+25,12*2,18,24,"CH");// C6H5CH3
				LCD_ShowNum(20+66,120+25+15,3,1,12); //显示一个数字		
			break;
			case 1://MQ2 可燃性气体 100~20000ppm 液化气(甲烷CH4丙烷C3H8)氢气 丁烷、丙烷、甲烷				
				POINT_COLOR=BROWN;//设置字体 画笔颜色	
				LCD_ShowString_user(95+130,72,24,"ppm");//显示一个字符串, 12/16/24字体 
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString_user(85+130,55,16,"/20000");//显示一个字符串, 12/16/24字体 
				POINT_COLOR=BLACK;//设置字体 画笔颜色	
				LCD_ShowString(20+130,120,12*2,18,24,"CH");// CH4
				LCD_ShowNum(20+130+2+12*2,120+15,4,1,12); //显示一个数字		
				LCD_ShowChar_user(130+20+92,120,'H',24,1) ;//H2
				LCD_ShowNum(130+20+90+12+3,120+15,2,1,12); //显示一个数字		
				LCD_ShowChar_user(130+20,120+25,'C',24,1) ;// C3H8
				LCD_ShowNum(130+20+12+1,120+25+15,3,1,12); //显示一个数字		
				LCD_ShowChar_user(130+20+18+2,120+25,'H',24,1) ;//显示一个字符
				LCD_ShowNum(130+20+18+4+12,120+25+15,8,1,12); //显示一个数字		
				break;
			case 2://MQ6   10~10000ppm 液化气、异丁烷、丙烷C3H8、LPG （丙烷、丁烷C4H10、液化石油气）
				POINT_COLOR=BROWN;//设置字体 画笔颜色	
				LCD_ShowString_user(95+130*2,72,24,"ppm");//显示一个字符串, 12/16/24字体 
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString_user(85+130*2,55,16,"/10000");//显示一个字符串, 12/16/24字体 
				POINT_COLOR=BLACK;//设置字体 画笔颜色	
				LCD_ShowChar_user(130*2+20,120,'C',24,1) ;// C3H8
				LCD_ShowNum(130*2+20+12+1,120+15,3,1,12); //显示一个数字		
				LCD_ShowChar_user(130*2+20+18+2,120,'H',24,1) ;//显示一个字符
				LCD_ShowNum(130*2+20+18+4+12,120+15,8,1,12); //显示一个数字		
				LCD_ShowChar_user(130*2+20,120+25,'C',24,1) ;// C4H10
				LCD_ShowNum(130*2+20+12+1,120+15+25,4,1,12); //显示一个数字		
				LCD_ShowChar_user(130*2+20+18+2,120+25,'H',24,1) ;//显示一个字符
				LCD_ShowNum(130*2+20+18+4+12,120+15+25,10,2,12); //显示一个数字		
				break;
			case 3://MQ7 CO：10~1000ppm  CO
				POINT_COLOR=BROWN;//设置字体 画笔颜色	
				LCD_ShowString_user(95,72+130,24,"ppm");//显示一个字符串, 12/16/24字体 
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString_user(95,55+130,16,"/1000");//显示一个字符串, 12/16/24字体 
				POINT_COLOR=BLACK;//设置字体 画笔颜色	
				LCD_ShowString(20,130+120,12*2,18,24,"CO");// CO
				break;
			case 4://内部温度值 
				POINT_COLOR=BROWN;//设置字体 画笔颜色	
				LCD_ShowString(15+130,51+40+130,24*2,16,16,"temp"); //显示一个字符
				LCD_ShowString_user(250,51+40+130,24,"C");//显示一个字符串, 12/16/24字体 
				break;
			case 5: /*温度测试*/
				POINT_COLOR=BROWN;//设置字体 画笔颜色	
				LCD_ShowString(15+130*2,51+40+130,24*2,16,16,"temp"); //显示一个字符
				LCD_ShowString_user(382,51+40+130,24,"C");//显示一个字符串, 12/16/24字体 
				break;
			case 6: /*湿度测试*/
				POINT_COLOR=BROWN;//设置字体 画笔颜色	
				LCD_ShowString(15+130*2,51+40+130+30,24*2,16,16,"humi"); //显示一个字符
				LCD_ShowString_user(382,51+40+130+30,24,"%");//显示一个字符串, 12/16/24字体 
				break;
		}		
	}
	flag_func=2;
}
////////////////////////////////////////////
void display_main(void)
{
	LCD_Clear(WHITE); //清屏
	BACK_COLOR=WHITE;  //背景色 
	POINT_COLOR=DARKBLUE;//设置字体 画笔颜色
	Picture_Draw(479-60,319-60,(u8 *) gImage_1 );//在指定座标范围显示一副图片
	Picture_Draw(0,0,(u8 *) gImage_2 );//在指定座标范围显示一副图片
	LCD_ShowString_user(45,3,16,"Sensor detection");//显示一个字符串, 12/16/24字体	   			
	Picture_Draw(140,60,(u8 *) gImage_eee );//在指定座标范围显示一副图片
	gui_fill_circle(210,299,22,LGRAY);//画实心圆
	BACK_COLOR=LGRAY;  //背景色 
	POINT_COLOR=BLACK;//设置字体 画笔颜色
//	LCD_ShowString(192,288,12*3,16,24,"OFF");
//	Picture_Draw(240,319-19,(u8 *) gImage_led );//在指定座标范围显示一副图片
	display_led_data();
	if(NRF_Check==1 )
	{
		BACK_COLOR=WHITE;  //背景色 
		POINT_COLOR=MAGENTA;//设置字体 画笔颜色
		LCD_Fill(318,3,440,24,WHITE); //填充单色
		LCD_ShowString_user(318,3,16,"NRF24L01-error");//显示一个字符串, 12/16/24字体	     
		Picture_Draw(479-40,0,(u8 *) gImage_plane );//在指定座标范围显示一副图片
	}
	if(NRF_Check==2 )
	{
		BACK_COLOR=WHITE;  //背景色 
		POINT_COLOR=DARKBLUE;//设置字体 画笔颜色
		LCD_Fill(318,3,440,24,WHITE); //填充单色
		LCD_ShowString_user(330,3,16,"NRF24L01-link");//显示一个字符串, 12/16/24字体	     
		Picture_Draw(479-40,0,(u8 *) gImage_wifi );//在指定座标范围显示一副图片
	}

	flag_func=1;
}
////////////////////////////////////////////
void display_col(void)
{
	LCD_Clear(WHITE); //清屏
	Picture_Draw(10,319-100-5,(u8 *) gImage_goback );//在指定座标范围显示一副图片
	Picture_Draw(479-100-10,319-100,(u8 *) gImage_left_right );//在指定座标范围显示一副图片
	Picture_Draw(240,319-19,(u8 *) gImage_led );//在指定座标范围显示一副图片
	Picture_Draw(479-41,2,(u8 *) gImage_return );//在指定座标范围显示一副图片
	Picture_Draw(140,60,(u8 *) gImage_eee );//在指定座标范围显示一副图片
	gui_fill_circle(210,299,22,LGRAY);//画实心圆
	BACK_COLOR=LGRAY;  //背景色 
	POINT_COLOR=BLACK;//设置字体 画笔颜色
//	LCD_ShowString(192,288,12*3,16,24,"OFF");
//	gui_fill_circle(41,41,40,YELLOW);//画实心圆
	BACK_COLOR=YELLOW;  //背景色 
	POINT_COLOR=BLACK;//设置字体 画笔颜色
	LCD_ShowString_user(9,28,16,"function");//显示一个字符串, 12/16/24字体	     
	flag_func=3;
	display_led_data();
}
////////////////////////////////////////////
u8 ledf=0;

void do_touch_function(void)
{
	static u8 ledd=0;
	static u8 jfdd=1;
	static u8 jfdd02=1;
	if( jfdd02==1 && jfdd==1 && ( tp_dev.x[0] < 40 )&&( tp_dev.y[0] > 479-40 )&&( tp_dev.y[0] < 480 ) ) 
	{
		display_sensor();
		jfdd=0;
	}
	if( jfdd02==1 && jfdd==0 && ( tp_dev.x[0] < 40 )&&( tp_dev.y[0] < 479-(479-65-30) )&&( tp_dev.y[0] > 479-(479-65) ) ) 
	{	
		display_main();
		jfdd=1;
	}
	if( jfdd02==1 &&( tp_dev.x[0] >319-60 )&&( tp_dev.x[0] <320 )&&( tp_dev.y[0] < 60 ) ) 
	{
		display_col();
		jfdd02=0;
	}	
	if( jfdd02==0 &&( tp_dev.x[0] <60 )&&( tp_dev.y[0] < 60 ) ) 
	{
		display_main();
		jfdd02=1;
		jfdd=1;
	}	
/**/	
	if( ledd ==0 && (flag_func==1||flag_func==3) &&( tp_dev.x[0] >240 ) //  led on
		&&( tp_dev.x[0] <320 )&&( tp_dev.y[0] > 240 )&&( tp_dev.y[0] < 300 ) ) 
	{
		ledd =1;
		ledf=1;
	}
	if( ledd ==1 && (flag_func==1||flag_func==3) &&( tp_dev.x[0] >240 ) // led off
		&&( tp_dev.x[0] <320 )&&( tp_dev.y[0] >170 )&&( tp_dev.y[0] < 230 ) ) 
	{
		ledd =0;
		ledf=1;
	}
/**/
	if(  flag_func==1 && ( tp_dev.x[0] <60 )&&( tp_dev.y[0] < 60 ) ) 
	{
		NRF_01(1);//  1:RX模式      ;    0:TX模式  
	}	
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

#else
void do_touch_function(void)
{}
#endif
void cmp_sensor(u16 x,u16 y,u8 *p,u16 set_numL,u16 set_numH)
{
	u16 cmp_num=0;
	if(p[2]!='.')
	{
		cmp_num=(p[0]-'0')*1000+(p[1]-'0')*100+(p[2]-'0')*10+(p[3]-'0');
		if(cmp_num<set_numL)
		{
			Picture_Draw(x,y,(u8 *) gImage_green );//在指定座标范围显示一副图片		
		}	
		else if(cmp_num<set_numH)
			{
				Picture_Draw(x,y,(u8 *) gImage_yellow );//在指定座标范围显示一副图片		
			}
			else
			{
				Picture_Draw(x,y,(u8 *) gImage_red );//在指定座标范围显示一副图片		
			}
	}
	else
	{
		cmp_num=(p[0]-'0')*10+(p[1]-'0');		
		if(cmp_num<set_numL-15 || cmp_num>set_numH+15)
		{
			Picture_Draw(x,y,(u8 *) gImage_red );//在指定座标范围显示一副图片		
		}	
		else if(cmp_num<set_numL || cmp_num>set_numH)
			{
				Picture_Draw(x,y,(u8 *) gImage_yellow );//在指定座标范围显示一副图片		
			}
			else
			{
				Picture_Draw(x,y,(u8 *) gImage_green );//在指定座标范围显示一副图片		
			}
	}
}
void display_sensor_data(void)
{
	u8 tt_buf[7][4]={0};
	char tyu[5]="00.00";
	u8 i=0;
	BACK_COLOR=LBBLUE;  //背景色 
	POINT_COLOR=CYAN;//设置字体 画笔颜色	
	for(i=0;i<7;i++)num_char(tt_buf[i],tx_buf,i);
	for(i=0;i<7;i++)
	{
		switch(i)
		{
			case 0:	//MQ135 氨气 烟雾 10-1000ppm 		
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString(20,80,24*2,18,24,tt_buf[i]);
				cmp_sensor(120,163,tt_buf[i],150,500);
				break;
			case 1://MQ2 可燃性气体 100~20000ppm 液化气(甲烷CH4丙烷C3H8)氢气 丁烷、丙烷、甲烷				
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString(20+130,80,24*2,18,24,tt_buf[i]);
				cmp_sensor(120+130,163,tt_buf[i],1000,5000);
				break;
			case 2://MQ6   10~10000ppm 液化气、异丁烷、丙烷C3H8、LPG （丙烷、丁烷C4H10、液化石油气）
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString(20+130*2,80,24*2,18,24,tt_buf[i]);
				cmp_sensor(120+130*2,163,tt_buf[i],150,400);
				break;
			case 3://MQ7 CO：10~1000ppm  CO
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString(20,80+130,24*2,18,24,tt_buf[i]);
				cmp_sensor(120,162+130,tt_buf[i],140,400);
				break;
			case 4://内部温度值 
				tyu[0]=tt_buf[i][1];
				tyu[1]=tt_buf[i][2];
				tyu[3]=tt_buf[i][3];
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString(70+130,51+40+130,24*2,24,24,tyu); //显示一个字符
				cmp_sensor(120+130,162+130,tyu,15,40);
				break;
			case 5: /*温度测试*/
				tyu[0]=tt_buf[i][0];
				tyu[1]=tt_buf[i][1];
				tyu[3]=tt_buf[i][2];
				tyu[4]=tt_buf[i][3];
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString_user(60+130*2,51+40+130,24,tyu);//显示一个字符串, 12/16/24字体 
				cmp_sensor(120+130*2,163+40,tyu,15,40);
				break;
			case 6: /*湿度测试*/
				tyu[0]=tt_buf[i][0];
				tyu[1]=tt_buf[i][1];
				tyu[3]=tt_buf[i][2];
				tyu[4]=tt_buf[i][3];
				POINT_COLOR=CYAN;//设置字体 画笔颜色	
				LCD_ShowString_user(60+130*2,51+40+130+30,24,tyu);//显示一个字符串, 12/16/24字体 
				cmp_sensor(120+130*2,163+115,tyu,60,80);
				break;
		}		
	}
}
//////////////////////////////////////////////
void num_char(u8 *buf_1,u8 *buf_2,u8 i)
{           
	buf_1[0]=buf_2[4*i+0];
	buf_1[1]=buf_2[4*i+1];
	buf_1[2]=buf_2[4*i+2];
	buf_1[3]=buf_2[4*i+3];
}
void display_main_data(void)
{}
void display_led_data(void)
{
	if(LED1==0)	
	{
		LCD_Fill(180,270,280,319,WHITE); //填充单色
		Picture_Draw(180,280,(u8 *) gImage_led_on );//在指定座标范围显示一副图片
		gui_fill_circle(250,299,22,MAGENTA);//画实心圆
		BACK_COLOR=MAGENTA;  //背景色 
		POINT_COLOR=BLACK;//设置字体 画笔颜色
		LCD_ShowString(236,288,12*3,16,24,"ON");
	}
	else 
	{
		LCD_Fill(180,270,280,319,WHITE); //填充单色
		Picture_Draw(240,319-19,(u8 *) gImage_led );//在指定座标范围显示一副图片
		gui_fill_circle(210,299,22,LGRAY);//画实心圆
		BACK_COLOR=LGRAY;  //背景色 
		POINT_COLOR=BLACK;//设置字体 画笔颜色
		LCD_ShowString(192,288,12*3,16,24,"OFF");
	}
	
}
void display_col_data(void)
{}
/************** RX模式 *****************/
void RX_(void)
{
	if( NRF24L01_RxPacket(tx_buf)==0)
	{
	}
	else {}	
}
/************** TX模式 *****************/
void TX_(void)
{
	if(LED1==0) tx_buf[0]='+';
	else tx_buf[0]='-';
	if( NRF24L01_TxPacket(tx_buf)==TX_OK )
	{
		nrf_d=1;
	}
	else 
	{
	}	
}

void NRF_check(void)
{
	if(NRF_Check==1 )
	{
		if( flag_func==1 && nrf_gg==0)
		{
			nrf_gg=1;
			BACK_COLOR=WHITE;  //背景色 
			POINT_COLOR=MAGENTA;//设置字体 画笔颜色
			LCD_Fill(318,3,440,24,WHITE); //填充单色
			LCD_ShowString_user(318,3,16,"NRF24L01-error");//显示一个字符串, 12/16/24字体	     
		}
		Picture_Draw(479-40,0,(u8 *) gImage_plane );//在指定座标范围显示一副图片
		bnb[0]=1;
		bnb[1]=0;
	}
	if(NRF_Check==2 )
	{
		if( flag_func==1 && nrf_gg==1)
		{
			nrf_gg=0;
			BACK_COLOR=WHITE;  //背景色 
			POINT_COLOR=DARKBLUE;//设置字体 画笔颜色
			LCD_Fill(318,3,440,24,WHITE); //填充单色
			LCD_ShowString_user(330,3,16,"NRF24L01-link");//显示一个字符串, 12/16/24字体	     
		}
		Picture_Draw(479-40,0,(u8 *) gImage_wifi );//在指定座标范围显示一副图片
		bnb[1]=1;
	}
	
}
