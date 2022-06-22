#include "timer.h"
#include "usbh_hid_Logitech.h"
#include "serial_bus.h"

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms    
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
				
//	TIM_Cmd(TIM3,ENABLE); //使能定时器3	
	TIM_Cmd(TIM3, DISABLE); 	
}

////定时器3中断服务函数
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET) //溢出中断
//	{
//#if 1		
//		_debug_log_info_c("X: %d\tY: %d\t", Logitech_Data.X, Logitech_Data.Y);
//		_debug_log_info_c("RZ: %d\t", Logitech_Data.RZ);
//		_debug_log_info_c("Slider: %d\t", Logitech_Data.Slider);
//		_debug_log_info_c("Hat_Switch: %d\t", Logitech_Data.Hat_Switch);
//		_debug_log_info_c("button: ");
//		for (uint16_t i=0x0001, j=1; i & 0x0fff ; i = i<<1, ++j) {
//			if (Logitech_Data.button & i) _debug_log_info_c("%d ", j);
//		}
//		_debug_log_info_c("\r\n") 
//#else		
//		sbus_out(&Logitech_Data);
//#endif
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
//}
