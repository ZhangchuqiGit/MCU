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

/*FreeRTOS头文件*/
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

//串口发送字符“AT+ROLE=1\r\n”，成功返回“OK\r\n”，其中\r\n 为回车换行
/*、 PIO11 模块状态切换脚,高电平-->AT 命令响应工作状态，低电平或悬空-->蓝牙常规工作状态
设置为主模块的步骤：
1、 PIO11 置高。
2、上电，模块进入 AT 命令响应状态。
3、超级终端或其他串口工具，设置波特率 38400，数据位 8 位，停止位 1 位，无校验位，
无流控制。
4、串口发送字符“AT+ROLE=1\r\n”，成功返回“OK\r\n”，其中\r\n 为回车换行。
5、 PIO 置低，重新上电，模块为主模块，自动搜索从模块，建立连接。
*/
void yuying_Run(void);//语音对话 -- 接收
void dht11(void);

#define start_prio 1		//任务优先级
#define start_size 128		//任务堆栈大小	
TaskHandle_t start_handle;	//任务句柄
void start_task(void *pvParameters);	//任务函数(申明)

#define yuyin_prio 3		//任务优先级
#define yuyin_size 128		//任务堆栈大小	
TaskHandle_t yuyin_handle;	//任务句柄
void yuyin_task(void *pvParameters);	//任务函数(申明)

#define cycle_prio 3		//任务优先级
#define cycle_size 256		//任务堆栈大小	
TaskHandle_t cycle_handle;	//任务句柄
void cycle_task(void * pvParameters);	//任务函数(申明)

int main(void)
{
/*把 STM32 的优先级分组的 4 个位均设成抢占优先级，也就是说完全放弃亚优先级。为何要这么设置？
其实这得怪 FreeRTOS 机构里面被驴踢过的逗逼，这些逗逼为了自己省事，直接默认不使用亚优先级   */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//但是在移植 FreeRTOS 时必须要修改成优先级分组方式 4	 
	delay_init();
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // 不使用JTAG调试，对应的IO口作为普通IO口使用
    OLED_Init();//初始化OLED                                            		+f   SCL--PA5             SDA--PA7
    OLED_DrawBMP(0,0,128,8,BMP1);//默认  
	/* 中断/溢出  时间 Tout us= ((arr+1)*(psc+1))/Tclk  */
	// TIM2 中断/溢出  时间 Tout us=   1000 * 72 / 72 = 1000 us
	TIM2_Int_Init(1000-1,720-1);// 10ms中断
	TIM1_PWM_Init(20000-1,72-1); //舵机的控制   用来产生PWM 频率  20ms = 50hz. 
	uart1_init(115200);//语音                                                           USART1_TX PA.9  RX  PA.10
	uart2_init(115200);//蓝牙 Android   												PA2 TXD2        PA3 RXD2   #&0001%
	uart3_init(9600);//舵机控制板                                                       PB10  TXD3      PB11 RXD3 
	SG90_out(1200);//上500-2500下   										舵机的控制					PB 13
	MG90S_out(1600);//右500-2500左  										舵机的控制					PB 14
	runActionGroup(0,1); //运行0号动作组1次     起立
    ADC1_Mode_init( );///stm32_adc转换，模拟输入端为  													PB0
	DHT11_Init();//																						PA11
    DS18B20_Init();// 																					PB9 

	Init_MPU3050();		     //初始化MPU3050                                         PB6	-I2C1_SCL、 PB7	-I2C1_SDA

//	InitMPU6050( ); //初始化MPU6050   											  
	/*创建任务*/
    xTaskCreate(start_task,						//任务函数
                "start_task",					//任务名称
				start_size,						//堆栈大小（字）
                NULL,							//传递给任务函数的参数
                start_prio,						//任务优先级
                &start_handle);					//任务句柄
    vTaskStartScheduler(); //开启任务调度
}
//开始任务
void start_task(void * pvParameters)
{
	taskENTER_CRITICAL();           //进入临界区+++++++++*
    //创建任务
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
				
    vTaskDelete(start_handle); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区----------*
}
u8 mmc=0;;
void cycle_task(void * pvParameters)	//任务函数(申明)
{
	while(1)
	{
//		if(USART2_led==1) //收到语音	
//		{
//			mmc=1;
//			USART2_led=0;
//			OLED_DrawBMP(0,0,128,8,BMP5);//说话
//			yuying_Android();//语音对话 -- 接收
////			printf("\t %s\r\n",USART2_RX_BUF);
//			USART1_led=0;
//		}	
//		else if(USART1_led==1) //收到语音	
//			{
//				mmc=1;
//				USART1_led=0;
//				OLED_DrawBMP(0,0,128,8,BMP5);//说话
//				yuying_Run( );//语音对话 -- 接收
//			}
//		printf("00000000\r\n");
		mmc=0;			
		vTaskDelay(1);
	}
	
}

//语音
void yuyin_task(void *pvParameters)	//任务函数(申明)
{
	while(1)
	{
		printf("\t\t\t\t\t ++++ \r\n");
		switch(mmc)
		{
			case 0: OLED_DrawBMP(0,0,128,8,BMP1);//默认
			break;
			case 1: OLED_DrawBMP(0,0,128,8,BMP5);//说话--收到语音	
			break;
			case 2: 
				case 3: OLED_DrawBMP(0,0,128,8,BMP2);//笑
			break;
			case 4: OLED_DrawBMP(0,0,128,8,BMP4);//无语
			break;
			case 5:
				case 6: OLED_DrawBMP(0,0,128,8,BMP3);//哭
			break;
			
		}
		taskENTER_CRITICAL();           //进入临界区+++++++++*
		sensor_task( ); 
		taskEXIT_CRITICAL();            //退出临界区----------*
		vTaskDelay(5); 
	}

}
/*				vTaskSuspend(TASK1Task_Handler);               //挂起任务1
			  printf("挂起任务1\r\n");
			  break;
			case KEY0_PRES:
				vTaskResume(TASK1Task_Handler);               //恢复任务1
*/
