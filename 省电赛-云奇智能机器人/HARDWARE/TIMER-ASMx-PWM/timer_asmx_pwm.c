/***************STM32F103C8T6**********************/
//接口 ：TIM1, CH1-PB13, CH2-PB14, //CH3-PB15
#include "timer_asmx_pwm.h"

/*        *****************************************************************
舵机的控制一般需要一个20ms （50hz）左右的时基脉冲，该脉冲的 高 电平部分一般为0.5ms-2.5ms范围内的角度控制脉冲部分，
总间隔为2ms。以180度角度伺服为例，那么对应的控制关系是这样的：
   0.5ms--------------0度；
   1.0ms------------45度；
   1.5ms------------90度；
   2.0ms-----------135度；
   2.5ms-----------180度；            对于stm32来说，我们可以使用定时器PWM输出来控制舵机。
*******************************************************   
PWM输出初始化  arr：自动重装值  psc：时钟预分频数     
// 中断/溢出  时间 Tout us= ((arr+1)*(psc+1))/Tclk
调节占空比= pwmval/10000 //1.5ms->90度   pwmval1=250-1;//500ms,   500-2500ms
	PWM输出频率的计算：
	PWM输出的是一个方波信号，信号的频率是由TIMx的时钟频率和TIMx_ARR这个寄存器所决定的,输出信号的占空比则是由TIMx_CRRx寄存器确定:
	占空比=(TIMx_CRRx/TIMx_ARR自动重装载寄存器)*100%    
	*/
void TIM1_PWM_Init(u16 arr,u16 psc)  //舵机的控制
//void TIM1_PWM_Init(void)  //舵机的控制
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure;  
    TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  		TIM_OCInitStructure;  
/**/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//定时器1时钟使能//GPIOB时钟使能
/**/ 
    TIM_DeInit(TIM1);//复位定时器1所有寄存器 
//设置该引脚为复用输出功能,输出PWM脉冲波形  
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 ; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  /*----++++++++++++++++------*/
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //推挽复用输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
/* Time base configuration */
// 中断/溢出  时间 Tout us= ((arr+1)*(psc+1))/Tclk (72) =20000 *72 /72 =20 000us=20ms
    TIM_TimeBaseStructure.TIM_Period = arr;//arr定时器周期,20ms
//	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;//psc时钟预分频
//	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //设置时钟分割:TDTS = Tck_tim,TIM_CKD_DIV1 =0x0
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数方式：向上计数
//初始化定时器
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位  
/*		   ---------	    	初始化TIMx Channelx PWM模式				------------		 */
// PWM1 Mode configuration: Channel1 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式：tim脉宽调制2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0 ;//****  CCR1_Val=0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//极性
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH1预装载使能
/* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0 ;//****  CCR2_Val=0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//极性
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH2预装载使能	
/* PWM1 Mode configuration: Channel3 */
/*
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH3预装载使能		*/
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器  
	TIM_CtrlPWMOutputs(TIM1,ENABLE);    //MOE 主输出使能  
	TIM_Cmd(TIM1, ENABLE);  //使能TIM4  
} 	
 

//舵机摆动
void SG90_out(u16 num)//500-2500  --  20 000
{
	TIM1->CCR1 = num;//pwm波高电平持续时间，2ms
}
//舵机摆动
void MG90S_out(u16 num)//500-2500  --  20 000
{
	TIM1->CCR2 = num;//pwm波高电平持续时间，2ms
}

/*************************************************定时器*******************************************************/
//通用定时器2中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M=72M
//arr：自动重装值。
//psc：时钟预分频数
void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
    TIM_DeInit(TIM2);//复位定时器2所有寄存器 

	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			//根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 					//使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  							//初始化NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  									//使能TIM2					 
	TIM_Cmd(TIM2, DISABLE);  						 
}
/*************************************************定时器*******************************************************/
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
    TIM_DeInit(TIM3);//复位定时器3所有寄存器 
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 						//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );						 //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  				//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  		//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  			//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  								//初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIM3					 
}
/***********************************************定时器*********************************************************/
//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
    TIM_DeInit(TIM4);//复位定时器4所有寄存器 
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 			//根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 					//使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  			//TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//先占优先级5级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  							//初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  									//使能TIM4					 
}

/* **************** STM32F103C8T6 定时器中断服务程序**************** */
u8 s_val=0;
u8 s=2;
u8 Flag=0;
u16 i=1200;//点头
u16 j=1600;//摇头
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		if(Flag==0)
		{
			i-=10;
			j-=10;
		}
		if(Flag==1)
		{
			i+=10;
			j+=10;
		}
		if(s_val==1)//点头
		{
			if(i<=950)
			{
				Flag=1;
			}
			if(i>=1450)
			{
				Flag=0;
			}
			if(i==1200)s--;
			SG90_out(i);//上500-2500下   PB 13
		}
		else if(s_val==2)//摇头
			{
				if(j<=1300)
				{
					Flag=1;
				}
				if(j>=1900)
				{
					Flag=0;
				}
				if(j==1600)s--;
				MG90S_out(j);//右500-2500左  PB 14
			}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
	if(s==0)
	{		
		if(s_val==2)MG90S_out(1600); 
		else if(s_val==1 )SG90_out(1200); 
		s_val=0;
		TIM_Cmd(TIM2, DISABLE); 
	}				 

}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		printf("TIM 3 输出.......\r\n");
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		printf("TIM 4 输出.......\r\n");
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}

