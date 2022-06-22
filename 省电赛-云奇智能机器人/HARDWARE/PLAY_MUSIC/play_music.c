#include "play_music.h"

uint16_t _value[4];
u8 mmc=0;

#if MPU6050_EN
Int16_xyz Accel,Gyro;	   //�����ۺϺ�Ĵ���������
Float_angle  Att_Angle;	 //ATT�������������̬��
static Int16_xyz mpu6050_dataacc,mpu6050_datagyr;
Int16_xyz	Acc_Data_Con;  //�˲���ļ��ٶ�

void sensor_0(void)
{
	MPU6050_GetDate();//MPU6050���ݻ�ȡ
	MPU6050_DataCon(&mpu6050_dataacc,&mpu6050_datagyr);	//2ms
	// 							acc���ٶ�      	  gyr���ٶ�	
	Accel.X = mpu6050_dataacc.X;
	Accel.Y = mpu6050_dataacc.Y;
	Accel.Z = mpu6050_dataacc.Z;
	Gyro.X = mpu6050_datagyr.X;
	Gyro.Y = mpu6050_datagyr.Y;
	Gyro.Z = mpu6050_datagyr.Z;

	Accel_Con( &Accel, &Gyro);//���ٶ��˲�
	IMUupdate( &Gyro, &Acc_Data_Con, &Att_Angle);	//IMU��̬����
	printf("������Pitch : %f \t �����Roll : %f \t �����Yaw %f \t\r\n",Att_Angle.pit,Att_Angle.rol,Att_Angle.yaw);
}
#endif


//������ 
void sensor_task(void)//������(����)
{
	u8 tempe_humi_data[4];
	DHT11_Read_Data(tempe_humi_data); //��ȡ��ʪ��ֵ  PA11					    
	_value[0]=tempe_humi_data[0]; //ʪ��
	_value[1]=ADC_GetConversionValue(ADC1);//MQ135--PB0 ����ֵ��: 0~4095(12λADC��2^12=4096) , 0~4095 �ɶ�Ӧ��ѹֵ 0~3.3V  
	_value[2]=DS18B20_Get_Temp()/100; // PB9  ���ȣ�1C ,�� 2788 /100 = 27 �¶�ֵ��-55.00~125.00�� 
//		printf("DS18B20 : %d\r\n",_value[3]);  
//		printf("ʪ�� : %d\r\n",tempe_humi_data[0]);  //ú�� > 2000
//		printf("ʪ�ȸ�λ : %d\r\n",_value[1]);  //ú�� > 2000
//		printf("ADC_GetConversionValue(ADC1) : %d\r\n",_value[2]);  //ú�� > 2000
//		_value= 3300000/4096*_value/1000; // 0~4095 �ɶ�Ӧ��ѹֵ 0~3.3V  


}

//+++++*********----/////////////////
void select_num(u8 num)
{
    switch(num)
    {
        case 101:
		printf("play,101,$");  
        break;
        case 102:
		printf("play,102,$");  
        break;
        case 103:
		printf("play,103,$");  
        break;
        case 104:
		printf("play,104,$");  
        break;
        case 105:
		printf("play,105,$");  
        break;
        case 106:
		printf("play,106,$");  
        break;
        case 107:
		printf("play,107,$");  
        break;
        case 108:
		printf("play,108,$");  
        break;
        case 109:
		printf("play,109,$");  
        break;
        case 110:
		printf("play,110,$");  
        break;
        case 111:
		printf("play,111,$");  
        break;
        case 112:
		printf("play,112,$");  
        break;
        case 113:
		printf("play,113,$");  
        break;
        case 114:
		printf("play,114,$");  
        break;
        case 115:
		printf("play,115,$");  
        break;
        case 116:
		printf("play,116,$");  
        break;
        case 117:
		printf("play,117,$");  
        break;
        case 118:
		printf("play,118,$");  
        break;
        case 119:
		printf("play,119,$");  
        break;
        case 120:
		printf("play,120,$");  
        break;
        case 121:
		printf("play,121,$");  
        break;
        case 122:
		printf("play,122,$");  
        break;
        case 123:
		printf("play,123,$");  
        break;
        case 124:
		printf("play,124,$");  	   
        break;
        case 125:
		printf("play,125,$");  
        break;
        case 126:
		printf("play,126,$");  
        break;
        case 127:
		printf("play,127,$");  
        break;
        case 128:
		printf("play,128,$");  
        break;
        case 129:
		printf("play,129,$");  
        break;
        case 130:
		printf("play,130,$");  
        break;
        case 131:
		printf("play,131,$");  
        break;
        case 132:
		printf("play,132,$");  
        break;
        case 133:
		printf("play,133,$");  
        break;
        case 134:
		printf("play,134,$");  
        break;
        case 135:
		printf("play,135,$");  
        break;
        case 136:
		printf("play,136,$");  
        break;
        case 137:
		printf("play,137,$");  
        break;
        case 138:
		printf("play,138,$");  
        break;
        case 139:
		printf("play,139,$");  
        break;
        case 140:
		printf("play,140,$");  
        break;
        case 141:
		printf("play,141,$");  
        break;
        case 142:
		printf("play,142,$");  
        break;
        case 143:
		printf("play,143,$");  
        break;
        case 144:
		printf("play,144,$");  
        break;
        case 145:
		printf("play,145,$");  
        break;
        case 146:
		printf("play,146,$");  
        break;
        case 147:
		printf("play,147,$");  
        break;
        case 148:
		printf("play,148,$");  
        break;
        case 149:
		printf("play,149,$");  
        break;
        case 150:
		printf("play,150,$");  
        break;
        case 151:
		printf("play,151,$");  
        break;
        case 152:
		printf("play,152,$");  
        break;
        case 153:
		printf("play,153,$");  
        break;
        case 154:
		printf("play,154,$");  
        break;
        case 155:
		printf("play,155,$");  
        break;
        case 156:
		printf("play,156,$");  
        break;
        case 157:
		printf("play,157,$");  
        break;
        case 158:
		printf("play,158,$");  
        break;
        case 159:
		printf("play,159,$");  
        break;
        case 160:
		printf("play,160,$");  
        break;
        case 161:
		printf("play,161,$");  
        break;
        case 162:
		printf("play,162,$");  
        break;
        case 163:
		printf("play,163,$");  
        break;
        case 164:
		printf("play,164,$");  
        break;
        case 165:
		printf("play,165,$");  
        break;
        case 166:
		printf("play,166,$");  
        break;
        case 167:
		printf("play,167,$");  
        break;
        case 168:
		printf("play,168,$");  
        break;
        case 169:
		printf("play,169,$");  
        break;
        case 170:
		printf("play,170,$");  
        break;
        case 171:
		printf("play,171,$");  
        break;
        case 172:
		printf("play,172,$");  
        break;
        case 173:
		printf("play,173,$");  
        break;
        case 174:
		printf("play,174,$");  
        break;
        case 175:
		printf("play,175,$");  
        break;
        case 176:
		printf("play,176,$");  
        break;
        case 177:
		printf("play,177,$");  
        break;
        case 178:
		printf("play,178,$");  
        break;
        case 179:
		printf("play,179,$");  
        break;
        case 180:
		printf("play,180,$");  
        break;
        case 181:
		printf("play,181,$");  
        break;
        case 182:
		printf("play,182,$");  
        break;
        case 183:
		printf("play,183,$");  
        break;
        case 184:
		printf("play,184,$");  
        break;
        case 185:
		printf("play,185,$");  
        break;
        case 186:
		printf("play,186,$");  
        break;
        case 187:
		printf("play,187,$");  
        break;
        case 188:
		printf("play,188,$");  
        break;
        case 189:
		printf("play,189,$");  
        break;
        case 190:
		printf("play,190,$");  
        break;
        case 191:
		printf("play,191,$");  
        break;
        case 192:
		printf("play,192,$");  
        break;
        case 193:
		printf("play,193,$");  
        break;
        case 194:
		printf("play,194,$");  
        break;
        case 195:
		printf("play,195,$");  
        break;
        case 196:
		printf("play,196,$");  
        break;
        case 197:
		printf("play,197,$");  
        break;
        case 198:
		printf("play,198,$");  
        break;
        case 199:
		printf("play,199,$");  
        break;
        case 200:
		printf("play,190,$");  
        break;

    };
	delay_ms(2000);//2000ms
}
void MQ135(void)//ú�� > 2000
{
    if(_value[1]<400)
    {
        printf("play,026,$"); 	mmc =2;     
    }
    else if(_value[1]<700)
		{
			printf("play,027,$"); mmc =3;    
		}
        else if(_value[1]<1000)
			{
				printf("play,028,$"); mmc =4;    
			}
            else if(_value[1]<1800)
				{
					printf("play,029,$");mmc =5;     
				}
				else  
					{
						printf("play,030,$"); mmc =6;    
					}

}
void ds18b20(void)
{
    if(_value[2]>2400&&_value[2]<2900)
    {
        OLED_DrawBMP(0,0,128,8,BMP2);//Ц
        printf("play,094,$");  //   
    }
    else if(_value[2]<=2400)
        {
            OLED_DrawBMP(0,0,128,8,BMP3);//��
            printf("play,096,$");  //   
        }
        else if(_value[2]>=2900)
            {
                OLED_DrawBMP(0,0,128,8,BMP4);//����
                printf("play,095,$");  //   
            }
}

//************
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
	int Android_select=0; 

	if( USART2_RX_BUF[USART2_RX_head+2]=='0' )
	{
			printf("!!!!!!! \t");

		Android_select = (int)(USART2_RX_BUF[USART2_RX_head+3]-'0')*100
						+(int)(USART2_RX_BUF[USART2_RX_head+4]-'0')*10
						+(int)(USART2_RX_BUF[USART2_RX_head+5]-'0');
		printf("    %d\r\n",Android_select);

		switch(Android_select)
        {
//			case 0:
//				printf("play,089,$");
//			break;
//			case 1: 
//				printf("play,089,$");
//			break;
//			case 2: 
//				printf("play,089,$");			
//            break;
//			case 3: 
//				printf("play,089,$");			
//            break;
//			case 4: 
//				printf("play,089,$");			
//            break;
//			case 5: 
//				printf("play,089,$");			
//            break;
//			case 6: 
//				printf("play,089,$");			
//            break;
//			case 7: 
//				printf("play,089,$");			
//            break;
//			case 8: 
//				printf("play,089,$");			
//            break;
//			case 9: 
//			
//            break;
//			case 10: 
//			
//            break;
//			case 11: 
//			
//            break;
//			case 12: 
//			
//            break;
//			case 13: 
//			
//            break;
//			case 14: 
//			
//            break;
//			case 15: 
//			
//            break;
//			case 16: 
//			
//            break;
//			case 17: 
//			
//            break;
//			case 18: 
//			
//            break;
//			case 19: 
//			
//            break;
//			case 20: 
//			
//            break;
//			case 21: 
//			
//            break;
//			case 22: 
//			
//            break;
			case 23: 
					printf("play,023,$"); //��ǰ�¶�Ϊ
					delay_ms(2500);
				    select_num(_value[0]);
					printf("play,021,$"); //��
            break;
			case 24: 
					printf("play,024,$"); //��ǰʪ��Ϊ
					delay_ms(2500);
				    select_num(_value[2]);
					ds18b20();
            break;
			case 25: 
					printf("play,025,$"); //��ǰ��������Ϊ
					delay_ms(3000);
					MQ135();
            break;
			case 26: 
			
            break;
			case 27: 
			
            break;
			case 28: 
			
            break;
			case 29: 
			
            break;
			case 30: 
			
            break;
			case 31: 
					runActionGroup(10,1); //����10�Ŷ�����1��
					printf("play,031,$"); //��ã��ܸ�����ʶ��
            break;
			case 32: 
					runActionGroup(11,1); //����10�Ŷ�����1��
					printf("play,032,$"); //��Һã�����С�ƣ�ϲ����������裬�ҵ������ǳ�Ϊ�����ܵĻ�����
            break;
			case 33: 
					printf("play,033,$"); //����������					
					delay_ms(2500);//2000ms
					printf("play,050,$"); //���֡��ɶ���
            break;
			case 34: 
					printf("play,034,$"); //�������赸
					delay_ms(2500);//2000ms
					runActionGroup(14,1); 
					printf("play,049,$"); //���֡�Сƻ����
            break;
			case 35: 
					printf("play,035,$"); //��������
            break;
			case 36: 
					printf("play,036,$"); //���ӳɹ�
            break;
			case 37:
					printf("play,037,$"); //����ʧ��
            break;
			case 38: 
					printf("play,038,$"); //�Ͽ�����
            break;
			case 39: 
					printf("play,039,$"); //��������
            break;
			case 40: 
					printf("play,040,$"); //һ�����
            break;
			case 41:
					printf("play,041,$"); //���µ�Ӱ�У����аɣ�����֮��
            break;
			case 42:
					printf("play,042,$"); //���ŵ�Ӱ��:�޳��衢�������׸������鹫Ԣ
            break;
			case 43:
					printf("play,043,$"); //һ��֮�����ڳ������Ϻ�
            break;
			case 44:
					printf("play,044,$"); //ÿ�����һ��㣬�����
            break;
			case 45:
					printf("play,045,$"); //Ը������������;�У����ն��ɿգ����Ϻã�
            break;
			case 46:
					printf("play,046,$"); //��8��21�յ�������Ԥ����
            break;
			case 47:
					printf("play,047,$"); //������ڣ�һ��Ҫ������Ŷ
            break;
//			case 48:
//					printf("play,048,$"); //�õ�
//            break;
//			case 49:
//					printf("play,049,$"); //���֡�Сƻ����
//            break;
//			case 50:
//					printf("play,050,$"); //���֡��ɶ���
//            break;
			case 51://����
					runActionGroup(15,1); //����15�Ŷ�����1��
            break;
			case 52://����
					printf("play,048,$"); //�õ�
					runActionGroup(9,1); //����15�Ŷ�����1��
            break;
			case 53://����
					runActionGroup(0,1); //����15�Ŷ�����1��
            break;
			case 54://ҡͷ    ��500-2500��
					MG90S_out(1600); 
					s_val=2;
					s=2; 
					j=1600;//ҡͷ
					TIM_Cmd(TIM2, ENABLE); //ʹ��TIM2					 
            break;
			case 55://��ͷ //��500-2500��
					SG90_out(1200); 
					s_val=1;
					s=2; 
					i=1200;//��ͷ
					TIM_Cmd(TIM2, ENABLE); //ʹ��TIM2					 
            break;
			case 56:

            break;
			case 57:

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

void yuying_Run(void)//�����Ի�
{
    int ct=0;
    if((USART1_RX_BUF[1]+USART1_RX_BUF[2])==USART1_RX_BUF[3])
    {
		ct=(int)USART1_RX_BUF[1]*10+(int)USART1_RX_BUF[2]; //����      
		switch(ct)
        {
			case 23: //ds18b20
					delay_ms(2500); //��ǰ�¶�Ϊ
				    select_num(_value[2]);
					printf("play,021,$"); //��
//					ds18b20();	
			break;
			case 24: //dht11
					delay_ms(3000);//��ǰʪ��Ϊ�ٷ�֮
				    select_num(_value[0]);
			break;
			case 25: //MQ135
					delay_ms(3000);//��ǰ��������Ϊ
					MQ135();
            break;
			case 31: //��ã��ܸ�����ʶ��
					runActionGroup(10,1); //����10�Ŷ�����1��
            break;
			case 32: //���ҽ��ܡ�����
					runActionGroup(11,1); //�Ϲ�
            break;
			case 33: 
					delay_ms(2500);//����������		
					printf("play,050,$"); //���֡��ɶ���
            break;
			case 34: 
					delay_ms(2500); //�������赸
					printf("play,049,$"); //���֡�Сƻ����
					runActionGroup(14,1); 
            break;
//			case 49:
//					printf("play,049,$"); //���֡�Сƻ����
//            break;
//			case 50:
//					printf("play,050,$"); //���֡��ɶ���
//            break;
			case 51://����
					runActionGroup(15,1); //����15�Ŷ�����1��
            break;
			case 52://����
					runActionGroup(9,1); //����15�Ŷ�����1��
            break;
			case 53://����
					runActionGroup(0,1); //����15�Ŷ�����1��
            break;
			case 54://ҡͷ    ��500-2500��
					MG90S_out(1600); 
					s_val=2;
					s=2; 
					j=1600;//ҡͷ
					TIM_Cmd(TIM2, ENABLE); //ʹ��TIM2					 
            break;
			case 55://��ͷ //��500-2500��
					SG90_out(1200); 
					s_val=1;
					s=2; 
					i=1200;//��ͷ
					TIM_Cmd(TIM2, ENABLE); //ʹ��TIM2					 
            break;

			case 15:  
/*	����ָ��������
void runActionGroup(uint8_t numOfAction, uint16_t Times)
							NumOfAction:���������
												Times:ִ�д���
												Times = 0 ʱ����ѭ��  */
					runActionGroup(7,1);
					delay_ms(3000);
			break;
 




		}
	}
	
}


