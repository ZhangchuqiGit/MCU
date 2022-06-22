#include "play_music.h"
#include "led.h"

u8 mmc=0;
u16 value=0;
//传感器 

void voice_go(u8 Times)
{
	u8 num=0;
	num=Times;
    runActionGroup(1,1); //运行1号动作组1次
	delay_ms(1000);
    runActionGroup(2,Times); //运行1号动作组Times次
	while(num--)delay_ms(1000);
    runActionGroup(3,1); //运行2号动作组1次
}//*************
void voice_back(u8 Times)
{
	u8 num=0;
	num=Times;
    runActionGroup(4,1); //运行4号动作组1次
	delay_ms(1000);
    runActionGroup(5,Times); //运行5号动作组Times次
	while(num--)delay_ms(1000);
    runActionGroup(6,1); //运行6号动作组1次
}//*************
void voice_left(u8 Times)
{
    runActionGroup(12,Times); //运行12号动作组Times次
}
void voice_right(u8 Times)
{
    runActionGroup(13,Times); //运行13号动作组Times次
}
void yuying_Android(void)//语音对话
{ 
	u8 Android_select=0; 
	u8 i=0;
			//01234567890
	char text1_tts[]={"度度度度度度度度度度度度度度度度度度度度度度度度度度度度度度度度度度度"};
	char text2_tts[]={"  度"};
	char text3_tts[]={"  %"};
	u8 text_1_tts[5];

	if(USART2_RX_BUF[USART2_RX_head+1]=='0') 
	{   
		LED = 0;	
		Android_select = (u8)(USART2_RX_BUF[USART2_RX_head+2]-'0')*100
						+(u8)(USART2_RX_BUF[USART2_RX_head+3]-'0')*10
						+(u8)(USART2_RX_BUF[USART2_RX_head+4]-'0')*1;
		switch(Android_select)
		{
			case 20: /* timer   */LED = 1;
				for(i=0;i<=USART2_RX_len;i++)
				{
					text1_tts[i]= (u8)USART2_RX_BUF[USART2_RX_head+i+5];
				}
				for(i=0;i<strlen(text1_tts);i++)
				{
					text1_tts[i]=' ';
				}
				Speech(text1_tts,5);
			break;
			case 23: 
				Speech("当前温度为26度",0);
//从DHT11读取一次数据//temp:温度值(范围:0~50°)//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
		    break;
			case 24: 
				Speech("当前湿度为71%",0);
			break;
			case 25: 
				Speech("当前空气质量为优秀",0);
			break;
			case 31: 
				Speech("你好，很高兴认识你",0);
			break;
			case 32: 
					runActionGroup(11,1); //运行10号动作组1次
				Speech("大家好，我是[2]小云，喜欢[2]唱歌和[2]跳舞，我的[2]梦想是成为最智能的机器人",0);
			break;
			case 33: 
					Speech("请欣赏[2]音乐",0);
					printf("play,050,$"); //音乐《成都》
			break;
			case 34: 
					Speech("请欣赏[2]舞蹈",0);
					runActionGroup(14,1); 
					printf("play,049,$"); //音乐《小苹果》
			break;
			case 35: 
					Speech("正在连接",0);
			break;
			case 36: 
					Speech("连接成功",0);
			break;
			case 37:
					Speech("连接失败",0);
			break;
			case 38: 
					Speech("断开连接",0);
			break;
			case 39: 
					Speech("天天[2]向上",0);
			break;
			case 40: 
					Speech("一起加油",0);
			break;
			case 41:
				Speech("最新电影有：旅行吧！井底之蛙",0);
			break;
			case 42:
				 Speech("热门电影有:巨齿鲨、西虹市首富、爱情公寓",0);
			break;
			case 43:
					Speech("[4]一日之计在于晨，[3]早上好",0);
			break;
			case 44:
					Speech("每天[2]进步一点点，[3]下午好",0);
			break;
			case 45:
					Speech("愿你在人生的旅途中，烦恼都成空，晚上好！",0);
			break;
			case 46:/*今天东莞市会有阵雨，气温为27至33摄氏度，风力为北风一级，空气质量为优秀*/
					Speech(text4_tts,0);
			break;
			case 47:
					Speech("后会有期，一定要来[3]看我哦！",0);
			break;
			case 51://握手
					runActionGroup(15,1); //运行15号动作组1次
			break;
			case 52://蹲下
					Speech("好的",0);//好的
					runActionGroup(9,1); //
			break;
			case 53://起立
					runActionGroup(0,1); //
			break;
			case 57:
					runActionGroup(8,1); //鞠躬
			break;
			case 58:

			break;
			case 59:

			break;
			case 60:
					voice_go(1);//前进1步
			break;
			case 61:
					voice_go(2);//前进2步
			break;
			case 62:
					voice_go(3);//前进3步
			break;
			case 63:
					voice_go(4);//前进4步
			break;
			case 64:
					voice_go(5);//前进5步
			break;
			case 65:
					voice_go(6);//前进6步
			break;
			case 66:
					voice_go(7);//前进7步
			break;
			case 67:

			break;
			case 68:

			break;
			case 69:

			break;
			case 70:
					voice_back(1);//后退1步
			break;
			case 71:
					voice_back(2);//后退2步
			break;
			case 72:
					voice_back(3);//后退3步
			break;
			case 73:
					voice_back(4);//后退4步
			break;
			case 74:
					voice_back(5);//后退5步
			break;
			case 75:
					voice_back(6);//后退6步
			break;
			case 76:
					voice_back(7);//后退7步
			break;
			case 77:

			break;
			case 78:

			break;
			case 79:

			break;
			case 80:
					voice_left(1);//向左转
			break;
			case 81:
					voice_right(1);//向右转
			break;
			case 82:
					voice_left(2);//2次向左转==向后转
			break;
			case 83:

			break;
			case 84:

			break;


		}		
	}	
}


