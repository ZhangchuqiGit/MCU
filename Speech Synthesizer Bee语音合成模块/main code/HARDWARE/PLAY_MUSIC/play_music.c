#include "play_music.h"
#include "led.h"

u8 mmc=0;
u16 value=0;
//������ 

void voice_go(u8 Times)
{
	u8 num=0;
	num=Times;
    runActionGroup(1,1); //����1�Ŷ�����1��
	delay_ms(1000);
    runActionGroup(2,Times); //����1�Ŷ�����Times��
	while(num--)delay_ms(1000);
    runActionGroup(3,1); //����2�Ŷ�����1��
}//*************
void voice_back(u8 Times)
{
	u8 num=0;
	num=Times;
    runActionGroup(4,1); //����4�Ŷ�����1��
	delay_ms(1000);
    runActionGroup(5,Times); //����5�Ŷ�����Times��
	while(num--)delay_ms(1000);
    runActionGroup(6,1); //����6�Ŷ�����1��
}//*************
void voice_left(u8 Times)
{
    runActionGroup(12,Times); //����12�Ŷ�����Times��
}
void voice_right(u8 Times)
{
    runActionGroup(13,Times); //����13�Ŷ�����Times��
}
void yuying_Android(void)//�����Ի�
{ 
	u8 Android_select=0; 
	u8 i=0;
			//01234567890
	char text1_tts[]={"�ȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶȶ�"};
	char text2_tts[]={"  ��"};
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
				Speech("��ǰ�¶�Ϊ26��",0);
//��DHT11��ȡһ������//temp:�¶�ֵ(��Χ:0~50��)//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
		    break;
			case 24: 
				Speech("��ǰʪ��Ϊ71%",0);
			break;
			case 25: 
				Speech("��ǰ��������Ϊ����",0);
			break;
			case 31: 
				Speech("��ã��ܸ�����ʶ��",0);
			break;
			case 32: 
					runActionGroup(11,1); //����10�Ŷ�����1��
				Speech("��Һã�����[2]С�ƣ�ϲ��[2]�����[2]���裬�ҵ�[2]�����ǳ�Ϊ�����ܵĻ�����",0);
			break;
			case 33: 
					Speech("������[2]����",0);
					printf("play,050,$"); //���֡��ɶ���
			break;
			case 34: 
					Speech("������[2]�赸",0);
					runActionGroup(14,1); 
					printf("play,049,$"); //���֡�Сƻ����
			break;
			case 35: 
					Speech("��������",0);
			break;
			case 36: 
					Speech("���ӳɹ�",0);
			break;
			case 37:
					Speech("����ʧ��",0);
			break;
			case 38: 
					Speech("�Ͽ�����",0);
			break;
			case 39: 
					Speech("����[2]����",0);
			break;
			case 40: 
					Speech("һ�����",0);
			break;
			case 41:
				Speech("���µ�Ӱ�У����аɣ�����֮��",0);
			break;
			case 42:
				 Speech("���ŵ�Ӱ��:�޳��衢�������׸������鹫Ԣ",0);
			break;
			case 43:
					Speech("[4]һ��֮�����ڳ���[3]���Ϻ�",0);
			break;
			case 44:
					Speech("ÿ��[2]����һ��㣬[3]�����",0);
			break;
			case 45:
					Speech("Ը������������;�У����ն��ɿգ����Ϻã�",0);
			break;
			case 46:/*���춫ݸ�л������꣬����Ϊ27��33���϶ȣ�����Ϊ����һ������������Ϊ����*/
					Speech(text4_tts,0);
			break;
			case 47:
					Speech("������ڣ�һ��Ҫ��[3]����Ŷ��",0);
			break;
			case 51://����
					runActionGroup(15,1); //����15�Ŷ�����1��
			break;
			case 52://����
					Speech("�õ�",0);//�õ�
					runActionGroup(9,1); //
			break;
			case 53://����
					runActionGroup(0,1); //
			break;
			case 57:
					runActionGroup(8,1); //�Ϲ�
			break;
			case 58:

			break;
			case 59:

			break;
			case 60:
					voice_go(1);//ǰ��1��
			break;
			case 61:
					voice_go(2);//ǰ��2��
			break;
			case 62:
					voice_go(3);//ǰ��3��
			break;
			case 63:
					voice_go(4);//ǰ��4��
			break;
			case 64:
					voice_go(5);//ǰ��5��
			break;
			case 65:
					voice_go(6);//ǰ��6��
			break;
			case 66:
					voice_go(7);//ǰ��7��
			break;
			case 67:

			break;
			case 68:

			break;
			case 69:

			break;
			case 70:
					voice_back(1);//����1��
			break;
			case 71:
					voice_back(2);//����2��
			break;
			case 72:
					voice_back(3);//����3��
			break;
			case 73:
					voice_back(4);//����4��
			break;
			case 74:
					voice_back(5);//����5��
			break;
			case 75:
					voice_back(6);//����6��
			break;
			case 76:
					voice_back(7);//����7��
			break;
			case 77:

			break;
			case 78:

			break;
			case 79:

			break;
			case 80:
					voice_left(1);//����ת
			break;
			case 81:
					voice_right(1);//����ת
			break;
			case 82:
					voice_left(2);//2������ת==���ת
			break;
			case 83:

			break;
			case 84:

			break;


		}		
	}	
}


