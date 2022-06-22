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
	LCD_Fill(0,0,479-65,319, LBBLUE); //��䵥ɫ
	Picture_Draw(479-65-39,0,(u8 *) gImage_3 );//��ָ�����귶Χ��ʾһ��ͼƬ
	Picture_Draw(479-60,319-60,(u8 *) gImage_1 );//��ָ�����귶Χ��ʾһ��ͼƬ
	BACK_COLOR=LBBLUE;  //����ɫ 
	POINT_COLOR=GREEN;//�������� ������ɫ
	LCD_ShowString_user(90,2,24,"Sensor detection");//��ʾһ���ַ���, 12/16/24����	     
	lcd_draw_bline(20,35,479-65-20-40,35,2,BROWN);//��һ������
	gui_draw_hline(10,50,390,WHITE);//��ˮƽ��1
	gui_draw_hline(10,50+130,390,WHITE);//��ˮƽ��2
	gui_draw_hline(10,319-10,390,WHITE);//��ˮƽ��3
	POINT_COLOR=WHITE;//�������� ������ɫ
	LCD_DrawLine(10,50,10,319-10); //����1
	LCD_DrawLine(10+130,50,10+130,319-10); //����2	
	LCD_DrawLine(10+130*2,50,10+130*2,319-10); //����3
	LCD_DrawLine(400,50,400,319-10); //����4
	POINT_COLOR=RED;//�������� ������ɫ
	LCD_ShowString_user(10+5,51,24,"MQ135");//��ʾһ���ַ���, 12/16/24����	     
	LCD_ShowString_user(10+130+5,51,24,"MQ2");//��ʾһ���ַ���, 12/16/24����	     
	LCD_ShowString_user(10+130*2+5,51,24,"MQ6");//��ʾһ���ַ���, 12/16/24����	     
	LCD_ShowString_user(15,50+130+2,24,"MQ7");//��ʾһ���ַ���, 12/16/24����	     
	LCD_ShowString_user(10+130+5,50+130+2,24,"sys_temp");//��ʾһ���ַ���, 12/16/24����	   			
	LCD_ShowString_user(10+130*2+5,50+130+2,24,"SHT20");//��ʾһ���ַ���, 12/16/24����	   			

	for(i=0;i<7;i++)
	{
		switch(i)
		{
			case 0:	//MQ135 ���� ���� 10-1000ppm 		
				POINT_COLOR=BROWN;//�������� ������ɫ	
				LCD_ShowString_user(95,72,24,"ppm");//��ʾһ���ַ���, 12/16/24���� 
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString_user(95,55,16,"/1000");//��ʾһ���ַ���, 12/16/24���� 
				POINT_COLOR=BLACK;//�������� ������ɫ	
				LCD_ShowString(20,120,12*2,18,24,"NH");// NH3
				LCD_ShowNum(20+2+12*2,120+15,3,1,12); //��ʾһ������		
				LCD_ShowChar_user(20+92,120,'H',24,1) ;//H2
				LCD_ShowNum(20+90+12+3,120+15,2,1,12); //��ʾһ������		
				LCD_ShowNum(20+1+12*2,120+15,3,1,12); //��ʾһ������		
				LCD_ShowChar_user(20,120+25,'C',24,1) ;// C6H5CH3
				LCD_ShowNum(20+12+1,120+25+15,6,1,12); //��ʾһ������		
				LCD_ShowChar_user(20+18+2,120+25,'H',24,1) ;//��ʾһ���ַ�
				LCD_ShowNum(20+18+4+12,120+25+15,5,1,12); //��ʾһ������		
				LCD_ShowString(20+12*3+4,120+25,12*2,18,24,"CH");// C6H5CH3
				LCD_ShowNum(20+66,120+25+15,3,1,12); //��ʾһ������		
			break;
			case 1://MQ2 ��ȼ������ 100~20000ppm Һ����(����CH4����C3H8)���� ���顢���顢����				
				POINT_COLOR=BROWN;//�������� ������ɫ	
				LCD_ShowString_user(95+130,72,24,"ppm");//��ʾһ���ַ���, 12/16/24���� 
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString_user(85+130,55,16,"/20000");//��ʾһ���ַ���, 12/16/24���� 
				POINT_COLOR=BLACK;//�������� ������ɫ	
				LCD_ShowString(20+130,120,12*2,18,24,"CH");// CH4
				LCD_ShowNum(20+130+2+12*2,120+15,4,1,12); //��ʾһ������		
				LCD_ShowChar_user(130+20+92,120,'H',24,1) ;//H2
				LCD_ShowNum(130+20+90+12+3,120+15,2,1,12); //��ʾһ������		
				LCD_ShowChar_user(130+20,120+25,'C',24,1) ;// C3H8
				LCD_ShowNum(130+20+12+1,120+25+15,3,1,12); //��ʾһ������		
				LCD_ShowChar_user(130+20+18+2,120+25,'H',24,1) ;//��ʾһ���ַ�
				LCD_ShowNum(130+20+18+4+12,120+25+15,8,1,12); //��ʾһ������		
				break;
			case 2://MQ6   10~10000ppm Һ�������춡�顢����C3H8��LPG �����顢����C4H10��Һ��ʯ������
				POINT_COLOR=BROWN;//�������� ������ɫ	
				LCD_ShowString_user(95+130*2,72,24,"ppm");//��ʾһ���ַ���, 12/16/24���� 
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString_user(85+130*2,55,16,"/10000");//��ʾһ���ַ���, 12/16/24���� 
				POINT_COLOR=BLACK;//�������� ������ɫ	
				LCD_ShowChar_user(130*2+20,120,'C',24,1) ;// C3H8
				LCD_ShowNum(130*2+20+12+1,120+15,3,1,12); //��ʾһ������		
				LCD_ShowChar_user(130*2+20+18+2,120,'H',24,1) ;//��ʾһ���ַ�
				LCD_ShowNum(130*2+20+18+4+12,120+15,8,1,12); //��ʾһ������		
				LCD_ShowChar_user(130*2+20,120+25,'C',24,1) ;// C4H10
				LCD_ShowNum(130*2+20+12+1,120+15+25,4,1,12); //��ʾһ������		
				LCD_ShowChar_user(130*2+20+18+2,120+25,'H',24,1) ;//��ʾһ���ַ�
				LCD_ShowNum(130*2+20+18+4+12,120+15+25,10,2,12); //��ʾһ������		
				break;
			case 3://MQ7 CO��10~1000ppm  CO
				POINT_COLOR=BROWN;//�������� ������ɫ	
				LCD_ShowString_user(95,72+130,24,"ppm");//��ʾһ���ַ���, 12/16/24���� 
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString_user(95,55+130,16,"/1000");//��ʾһ���ַ���, 12/16/24���� 
				POINT_COLOR=BLACK;//�������� ������ɫ	
				LCD_ShowString(20,130+120,12*2,18,24,"CO");// CO
				break;
			case 4://�ڲ��¶�ֵ 
				POINT_COLOR=BROWN;//�������� ������ɫ	
				LCD_ShowString(15+130,51+40+130,24*2,16,16,"temp"); //��ʾһ���ַ�
				LCD_ShowString_user(250,51+40+130,24,"C");//��ʾһ���ַ���, 12/16/24���� 
				break;
			case 5: /*�¶Ȳ���*/
				POINT_COLOR=BROWN;//�������� ������ɫ	
				LCD_ShowString(15+130*2,51+40+130,24*2,16,16,"temp"); //��ʾһ���ַ�
				LCD_ShowString_user(382,51+40+130,24,"C");//��ʾһ���ַ���, 12/16/24���� 
				break;
			case 6: /*ʪ�Ȳ���*/
				POINT_COLOR=BROWN;//�������� ������ɫ	
				LCD_ShowString(15+130*2,51+40+130+30,24*2,16,16,"humi"); //��ʾһ���ַ�
				LCD_ShowString_user(382,51+40+130+30,24,"%");//��ʾһ���ַ���, 12/16/24���� 
				break;
		}		
	}
	flag_func=2;
}
////////////////////////////////////////////
void display_main(void)
{
	LCD_Clear(WHITE); //����
	BACK_COLOR=WHITE;  //����ɫ 
	POINT_COLOR=DARKBLUE;//�������� ������ɫ
	Picture_Draw(479-60,319-60,(u8 *) gImage_1 );//��ָ�����귶Χ��ʾһ��ͼƬ
	Picture_Draw(0,0,(u8 *) gImage_2 );//��ָ�����귶Χ��ʾһ��ͼƬ
	LCD_ShowString_user(45,3,16,"Sensor detection");//��ʾһ���ַ���, 12/16/24����	   			
	Picture_Draw(140,60,(u8 *) gImage_eee );//��ָ�����귶Χ��ʾһ��ͼƬ
	gui_fill_circle(210,299,22,LGRAY);//��ʵ��Բ
	BACK_COLOR=LGRAY;  //����ɫ 
	POINT_COLOR=BLACK;//�������� ������ɫ
//	LCD_ShowString(192,288,12*3,16,24,"OFF");
//	Picture_Draw(240,319-19,(u8 *) gImage_led );//��ָ�����귶Χ��ʾһ��ͼƬ
	display_led_data();
	if(NRF_Check==1 )
	{
		BACK_COLOR=WHITE;  //����ɫ 
		POINT_COLOR=MAGENTA;//�������� ������ɫ
		LCD_Fill(318,3,440,24,WHITE); //��䵥ɫ
		LCD_ShowString_user(318,3,16,"NRF24L01-error");//��ʾһ���ַ���, 12/16/24����	     
		Picture_Draw(479-40,0,(u8 *) gImage_plane );//��ָ�����귶Χ��ʾһ��ͼƬ
	}
	if(NRF_Check==2 )
	{
		BACK_COLOR=WHITE;  //����ɫ 
		POINT_COLOR=DARKBLUE;//�������� ������ɫ
		LCD_Fill(318,3,440,24,WHITE); //��䵥ɫ
		LCD_ShowString_user(330,3,16,"NRF24L01-link");//��ʾһ���ַ���, 12/16/24����	     
		Picture_Draw(479-40,0,(u8 *) gImage_wifi );//��ָ�����귶Χ��ʾһ��ͼƬ
	}

	flag_func=1;
}
////////////////////////////////////////////
void display_col(void)
{
	LCD_Clear(WHITE); //����
	Picture_Draw(10,319-100-5,(u8 *) gImage_goback );//��ָ�����귶Χ��ʾһ��ͼƬ
	Picture_Draw(479-100-10,319-100,(u8 *) gImage_left_right );//��ָ�����귶Χ��ʾһ��ͼƬ
	Picture_Draw(240,319-19,(u8 *) gImage_led );//��ָ�����귶Χ��ʾһ��ͼƬ
	Picture_Draw(479-41,2,(u8 *) gImage_return );//��ָ�����귶Χ��ʾһ��ͼƬ
	Picture_Draw(140,60,(u8 *) gImage_eee );//��ָ�����귶Χ��ʾһ��ͼƬ
	gui_fill_circle(210,299,22,LGRAY);//��ʵ��Բ
	BACK_COLOR=LGRAY;  //����ɫ 
	POINT_COLOR=BLACK;//�������� ������ɫ
//	LCD_ShowString(192,288,12*3,16,24,"OFF");
//	gui_fill_circle(41,41,40,YELLOW);//��ʵ��Բ
	BACK_COLOR=YELLOW;  //����ɫ 
	POINT_COLOR=BLACK;//�������� ������ɫ
	LCD_ShowString_user(9,28,16,"function");//��ʾһ���ַ���, 12/16/24����	     
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
		NRF_01(1);//  1:RXģʽ      ;    0:TXģʽ  
	}	
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
			Picture_Draw(x,y,(u8 *) gImage_green );//��ָ�����귶Χ��ʾһ��ͼƬ		
		}	
		else if(cmp_num<set_numH)
			{
				Picture_Draw(x,y,(u8 *) gImage_yellow );//��ָ�����귶Χ��ʾһ��ͼƬ		
			}
			else
			{
				Picture_Draw(x,y,(u8 *) gImage_red );//��ָ�����귶Χ��ʾһ��ͼƬ		
			}
	}
	else
	{
		cmp_num=(p[0]-'0')*10+(p[1]-'0');		
		if(cmp_num<set_numL-15 || cmp_num>set_numH+15)
		{
			Picture_Draw(x,y,(u8 *) gImage_red );//��ָ�����귶Χ��ʾһ��ͼƬ		
		}	
		else if(cmp_num<set_numL || cmp_num>set_numH)
			{
				Picture_Draw(x,y,(u8 *) gImage_yellow );//��ָ�����귶Χ��ʾһ��ͼƬ		
			}
			else
			{
				Picture_Draw(x,y,(u8 *) gImage_green );//��ָ�����귶Χ��ʾһ��ͼƬ		
			}
	}
}
void display_sensor_data(void)
{
	u8 tt_buf[7][4]={0};
	char tyu[5]="00.00";
	u8 i=0;
	BACK_COLOR=LBBLUE;  //����ɫ 
	POINT_COLOR=CYAN;//�������� ������ɫ	
	for(i=0;i<7;i++)num_char(tt_buf[i],tx_buf,i);
	for(i=0;i<7;i++)
	{
		switch(i)
		{
			case 0:	//MQ135 ���� ���� 10-1000ppm 		
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString(20,80,24*2,18,24,tt_buf[i]);
				cmp_sensor(120,163,tt_buf[i],150,500);
				break;
			case 1://MQ2 ��ȼ������ 100~20000ppm Һ����(����CH4����C3H8)���� ���顢���顢����				
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString(20+130,80,24*2,18,24,tt_buf[i]);
				cmp_sensor(120+130,163,tt_buf[i],1000,5000);
				break;
			case 2://MQ6   10~10000ppm Һ�������춡�顢����C3H8��LPG �����顢����C4H10��Һ��ʯ������
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString(20+130*2,80,24*2,18,24,tt_buf[i]);
				cmp_sensor(120+130*2,163,tt_buf[i],150,400);
				break;
			case 3://MQ7 CO��10~1000ppm  CO
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString(20,80+130,24*2,18,24,tt_buf[i]);
				cmp_sensor(120,162+130,tt_buf[i],140,400);
				break;
			case 4://�ڲ��¶�ֵ 
				tyu[0]=tt_buf[i][1];
				tyu[1]=tt_buf[i][2];
				tyu[3]=tt_buf[i][3];
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString(70+130,51+40+130,24*2,24,24,tyu); //��ʾһ���ַ�
				cmp_sensor(120+130,162+130,tyu,15,40);
				break;
			case 5: /*�¶Ȳ���*/
				tyu[0]=tt_buf[i][0];
				tyu[1]=tt_buf[i][1];
				tyu[3]=tt_buf[i][2];
				tyu[4]=tt_buf[i][3];
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString_user(60+130*2,51+40+130,24,tyu);//��ʾһ���ַ���, 12/16/24���� 
				cmp_sensor(120+130*2,163+40,tyu,15,40);
				break;
			case 6: /*ʪ�Ȳ���*/
				tyu[0]=tt_buf[i][0];
				tyu[1]=tt_buf[i][1];
				tyu[3]=tt_buf[i][2];
				tyu[4]=tt_buf[i][3];
				POINT_COLOR=CYAN;//�������� ������ɫ	
				LCD_ShowString_user(60+130*2,51+40+130+30,24,tyu);//��ʾһ���ַ���, 12/16/24���� 
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
		LCD_Fill(180,270,280,319,WHITE); //��䵥ɫ
		Picture_Draw(180,280,(u8 *) gImage_led_on );//��ָ�����귶Χ��ʾһ��ͼƬ
		gui_fill_circle(250,299,22,MAGENTA);//��ʵ��Բ
		BACK_COLOR=MAGENTA;  //����ɫ 
		POINT_COLOR=BLACK;//�������� ������ɫ
		LCD_ShowString(236,288,12*3,16,24,"ON");
	}
	else 
	{
		LCD_Fill(180,270,280,319,WHITE); //��䵥ɫ
		Picture_Draw(240,319-19,(u8 *) gImage_led );//��ָ�����귶Χ��ʾһ��ͼƬ
		gui_fill_circle(210,299,22,LGRAY);//��ʵ��Բ
		BACK_COLOR=LGRAY;  //����ɫ 
		POINT_COLOR=BLACK;//�������� ������ɫ
		LCD_ShowString(192,288,12*3,16,24,"OFF");
	}
	
}
void display_col_data(void)
{}
/************** RXģʽ *****************/
void RX_(void)
{
	if( NRF24L01_RxPacket(tx_buf)==0)
	{
	}
	else {}	
}
/************** TXģʽ *****************/
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
			BACK_COLOR=WHITE;  //����ɫ 
			POINT_COLOR=MAGENTA;//�������� ������ɫ
			LCD_Fill(318,3,440,24,WHITE); //��䵥ɫ
			LCD_ShowString_user(318,3,16,"NRF24L01-error");//��ʾһ���ַ���, 12/16/24����	     
		}
		Picture_Draw(479-40,0,(u8 *) gImage_plane );//��ָ�����귶Χ��ʾһ��ͼƬ
		bnb[0]=1;
		bnb[1]=0;
	}
	if(NRF_Check==2 )
	{
		if( flag_func==1 && nrf_gg==1)
		{
			nrf_gg=0;
			BACK_COLOR=WHITE;  //����ɫ 
			POINT_COLOR=DARKBLUE;//�������� ������ɫ
			LCD_Fill(318,3,440,24,WHITE); //��䵥ɫ
			LCD_ShowString_user(330,3,16,"NRF24L01-link");//��ʾһ���ַ���, 12/16/24����	     
		}
		Picture_Draw(479-40,0,(u8 *) gImage_wifi );//��ָ�����귶Χ��ʾһ��ͼƬ
		bnb[1]=1;
	}
	
}
