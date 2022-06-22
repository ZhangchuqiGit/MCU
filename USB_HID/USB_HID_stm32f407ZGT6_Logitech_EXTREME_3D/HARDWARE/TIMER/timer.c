#include "timer.h"
#include "usbh_hid_Logitech.h"
#include "serial_bus.h"

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms    
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
				
//	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3	
	TIM_Cmd(TIM3, DISABLE); 	
}

////��ʱ��3�жϷ�����
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET) //����ж�
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
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
//}
