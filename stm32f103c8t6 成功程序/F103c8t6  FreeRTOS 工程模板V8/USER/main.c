#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "malloc.h"

/*******************************************************************************
#if
#elif
#elif
#else
#endif
*******************************************************************************/

/*FreeRTOSͷ�ļ�*/
#include "FreeRTOS.h"
#include "task.h"

#include "adc.h"
#include "oled.h"
#include "dht11.h"
#include "adc.h"
#include "math.h"
#include "ds18b20.h"
#include "timer_asmx_pwm.h"
#include "LobotServoController.h"
#include "play_music.h"
#include "I2C_MPU6050.h"
//#include "myimu.h"

//���ڷ����ַ���AT+ROLE=1\r\n�����ɹ����ء�OK\r\n��������\r\n Ϊ�س�����
/*�� PIO11 ģ��״̬�л���,�ߵ�ƽ-->AT ������Ӧ����״̬���͵�ƽ������-->�������湤��״̬
����Ϊ��ģ��Ĳ��裺
1�� PIO11 �øߡ�
2���ϵ磬ģ����� AT ������Ӧ״̬��
3�������ն˻��������ڹ��ߣ����ò����� 38400������λ 8 λ��ֹͣλ 1 λ����У��λ��
�������ơ�
4�����ڷ����ַ���AT+ROLE=1\r\n�����ɹ����ء�OK\r\n��������\r\n Ϊ�س����С�
5�� PIO �õͣ������ϵ磬ģ��Ϊ��ģ�飬�Զ�������ģ�飬�������ӡ�
*/
void yuying_Run(void);//�����Ի� -- ����
void dht11(void);

#define start_prio 1		//�������ȼ�
#define start_size 128		//�����ջ��С	
TaskHandle_t start_handle;	//������
void start_task(void *pvParameters);	//������(����)

#define yuyin_prio 3		//�������ȼ�
#define yuyin_size 128		//�����ջ��С	
TaskHandle_t yuyin_handle;	//������
void yuyin_task(void *pvParameters);	//������(����)

#define cycle_prio 3		//�������ȼ�
#define cycle_size 256		//�����ջ��С	
TaskHandle_t cycle_handle;	//������
void cycle_task(void * pvParameters);	//������(����)

int main(void)
{
/*�� STM32 �����ȼ������ 4 ��λ�������ռ���ȼ���Ҳ����˵��ȫ���������ȼ���Ϊ��Ҫ��ô���ã�
��ʵ��ù� FreeRTOS �������汻¿�߹��Ķ��ƣ���Щ����Ϊ���Լ�ʡ�£�ֱ��Ĭ�ϲ�ʹ�������ȼ�   */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//��������ֲ FreeRTOS ʱ����Ҫ�޸ĳ����ȼ����鷽ʽ 4	 
	delay_init();
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // ��ʹ��JTAG���ԣ���Ӧ��IO����Ϊ��ͨIO��ʹ��
    OLED_Init();//��ʼ��OLED                                            		+f   SCL--PA5             SDA--PA7
    OLED_DrawBMP(0,0,128,8,BMP1);//Ĭ��  
	/* �ж�/���  ʱ�� Tout us= ((arr+1)*(psc+1))/Tclk  */
	// TIM2 �ж�/���  ʱ�� Tout us=   1000 * 72 / 72 = 1000 us
	TIM2_Int_Init(1000-1,720-1);// 10ms�ж�
	TIM1_PWM_Init(20000-1,72-1); //����Ŀ���   ��������PWM Ƶ��  20ms = 50hz. 
	uart1_init(115200);//����                                                           USART1_TX PA.9  RX  PA.10
	uart2_init(115200);//���� Android   												PA2 TXD2        PA3 RXD2   #&0001%
	uart3_init(9600);//������ư�                                                       PB10  TXD3      PB11 RXD3 
	SG90_out(1200);//��500-2500��   										����Ŀ���					PB 13
	MG90S_out(1600);//��500-2500��  										����Ŀ���					PB 14
	runActionGroup(0,1); //����0�Ŷ�����1��     ����
    ADC1_Mode_init( );///stm32_adcת����ģ�������Ϊ  													PB0
	DHT11_Init();//																						PA11
    DS18B20_Init();// 																					PB9 

	Init_MPU3050();		     //��ʼ��MPU3050                                         PB6	-I2C1_SCL�� PB7	-I2C1_SDA

//	InitMPU6050( ); //��ʼ��MPU6050   											  
	/*��������*/
    xTaskCreate(start_task,						//������
                "start_task",					//��������
				start_size,						//��ջ��С���֣�
                NULL,							//���ݸ��������Ĳ���
                start_prio,						//�������ȼ�
                &start_handle);					//������
    vTaskStartScheduler(); //�����������
}
//��ʼ����
void start_task(void * pvParameters)
{
	taskENTER_CRITICAL();           //�����ٽ���+++++++++*
    //��������
    xTaskCreate(yuyin_task,             
                "led_task",           
                yuyin_size,        
                NULL,                  
                yuyin_prio,        
                &yuyin_handle);   
	xTaskCreate(cycle_task,
				"cycle_task",
				cycle_size,
				NULL,
				cycle_prio, 
				&cycle_handle);
				
    vTaskDelete(start_handle); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���----------*
}
u8 mmc=0;;
void cycle_task(void * pvParameters)	//������(����)
{
	while(1)
	{
//		if(USART2_led==1) //�յ�����	
//		{
//			mmc=1;
//			USART2_led=0;
//			OLED_DrawBMP(0,0,128,8,BMP5);//˵��
//			yuying_Android();//�����Ի� -- ����
////			printf("\t %s\r\n",USART2_RX_BUF);
//			USART1_led=0;
//		}	
//		else if(USART1_led==1) //�յ�����	
//			{
//				mmc=1;
//				USART1_led=0;
//				OLED_DrawBMP(0,0,128,8,BMP5);//˵��
//				yuying_Run( );//�����Ի� -- ����
//			}
//		printf("00000000\r\n");
		mmc=0;			
		vTaskDelay(1);
	}
	
}

//����
void yuyin_task(void *pvParameters)	//������(����)
{
	while(1)
	{
		printf("\t\t\t\t\t ++++ \r\n");
		switch(mmc)
		{
			case 0: OLED_DrawBMP(0,0,128,8,BMP1);//Ĭ��
			break;
			case 1: OLED_DrawBMP(0,0,128,8,BMP5);//˵��--�յ�����	
			break;
			case 2: 
				case 3: OLED_DrawBMP(0,0,128,8,BMP2);//Ц
			break;
			case 4: OLED_DrawBMP(0,0,128,8,BMP4);//����
			break;
			case 5:
				case 6: OLED_DrawBMP(0,0,128,8,BMP3);//��
			break;
			
		}
		taskENTER_CRITICAL();           //�����ٽ���+++++++++*
		sensor_task( ); 
		taskEXIT_CRITICAL();            //�˳��ٽ���----------*
		vTaskDelay(5); 
	}

}
/*				vTaskSuspend(TASK1Task_Handler);               //��������1
			  printf("��������1\r\n");
			  break;
			case KEY0_PRES:
				vTaskResume(TASK1Task_Handler);               //�ָ�����1
*/
