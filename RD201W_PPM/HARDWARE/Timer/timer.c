/***************STM32F103C8T6**********************
 * 文件名  ：timer.c
 * 描述    : TIMx定时器
 * 实验平台：STM32F103C8T6
 * 备注    ：使用定时器来产生pwm波，控制舵机动作
 * 接口    ：TIM1, CH1-PB13, CH2-PB14, //CH3-PB15
****************STM32F103C8T6**********************/
#include "timer.h"
#include "misc.h"
#include "usart.h"

/***************
中断/溢出  时间 t=  (  72M /(TIM_Prescaler+1)  )  *  (  TIM_Period+1  )
void TIM2_Int_Init(u16 arr,u16 psc)
//   TIM2_Int_Init(1000-1,72-1);
1000-1    72-1  = 1ms 
中断周期为1ms
void TIM3_Int_Init(u16 arr,u16 psc)
//  TIM3_Int_Init(5000-1,7200-1);//10Khz的计数频率，计数到5000为50ms 
// 中断/溢出  时间 t = ( 72M /(psc+1) ) * ( arr+1 ) = 72M * arr / psc
				   t = ( 72M / 7200 ) * ( 5000 ) = 72M * 5000 / 7200 = 50ms
				   t = ( 72M / 72 ) * ( 20000 ) = 72M * 20 000 / 72 = 20ms
****************/

#if 0

//-----------------------------------------------------------------
//----------------------------------------------------------------
/*配置TIM1输出的PWM信号的模式，如周期、极性、占空比 */
void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	u16 CCR1_Val = 0;        
	u16 CCR2_Val = 0;
	//u16 CCR3_Val = 50000;


/* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1/* arr */;//定时器周期,20ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;/* psc *///时钟预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数方式：向上计数

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

/* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式：tim脉宽调制2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//极性
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

/* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	

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
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	*/

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

void TIM1_Int_Init()/* 配置TIM1复用输出PWM时用到的I/O  */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOB, ENABLE);//定时器1时钟使能//GPIOB时钟使能

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 ;//| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*配置和初始化TIM1 */
void TIM1_PWM_Init(void)
{
	TIM1_Int_Init();
	TIM1_Mode_Config();	
}

//-----------------------------------------------------------------
//----------------------- TIM2 ---------------------------------
void TIM2_Int_Init(u16 arr,u16 psc)
{
//定时器TIM2初始化   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//中断优先级NVIC设置
	NVIC_InitTypeDef NVIC_InitStructure; 
    
//定时器TIM2初始化   **************************************************  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);//时钟使能
    
    TIM_DeInit(TIM2);//复位定时器2所有寄存器
    
    TIM_TimeBaseStructure.TIM_Period=arr;	//自动重装载寄存器周期的值
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	//时钟预分频数 72M/72      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
/*位9:8 CKD[1:0]: 时钟分频因子定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)使用的采样频率之间的分频比例。*/
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
   
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// 清除溢出中断标志 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//使能指定的TIM2中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	   //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能   
    NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
 
    TIM_Cmd(TIM2, ENABLE);	// 开启时钟    
    
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//先关闭等待使用      
/*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//先关闭  //使能命令  使能TIME2的时钟
    TIM_Cmd(TIM2, ENABLE);	// 开启时钟      */
}

//-----------------------------------------------------------------
//----------------------------------------------------------------
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
//定时器TIM3初始化   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//中断优先级NVIC设置
   NVIC_InitTypeDef NVIC_InitStructure; 
    
//定时器TIM3初始化   **************************************************  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
    TIM_DeInit(TIM3);//复位定时器3所有寄存器

    TIM_TimeBaseStructure.TIM_Period=arr;	//自动重装载寄存器周期的值
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	//时钟预分频数 72M/72      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
/*位9:8 CKD[1:0]: 时钟分频因子定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)使用的采样频率之间的分频比例。*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);	// 清除溢出中断标志 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能指定的TIM3中断,允许更新中断

 //中断优先级NVIC设置   **************************************************     
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx		// 开启时钟    
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);	//先关闭等待使用      
    /*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);	//先关闭  //使能命令  使能TIME2的时钟
    TIM_Cmd(TIM3, ENABLE);	// 开启时钟      
*/
}


//-----------------------------------------------------------------
//----------------------------------------------------------------
//定时器1中断服务程序
void TIM1_IRQHandler(void)   //TIM1中断
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM1更新中断发生与否
    {
            //////////***//////////////
//         printf(" //定时器 1 中断服务程序\r\n");            
            //////////***//////////////
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx更新中断标志 
}

//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
            //////////***//////////////
//         printf(" //定时器 2 中断服务程序\r\n");            
            //////////***//////////////
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{

            //////////***//////////////
//         printf(" //定时器 3 中断服务程序\r\n");  
            //////////***//////////////
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
}

#endif


#if 1

/* 定时器2 通道2 (PA1) 输入 PPM 捕获配置 */
//TIM2_Capture_PPM_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
void TIM2_Capture_PPM_Init() // 捕获 遥控器 PPM 几通道
{
	EXTI_InitTypeDef EXTI_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定时器TIM2初始化   
	NVIC_InitTypeDef NVIC_InitStructure; 	//中断优先级NVIC设置
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOA时钟

	//gpio初始化：
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

    TIM_DeInit(TIM2);//复位定时器2所有寄存器
	
	//初始化定时器2 TIM2	 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =72; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//输入捕获初始化：
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; //由于先检测低电平的同步头，所以这里下降沿触发
  	//下降沿触发
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM_ICInitStructure.TIM_ICFilter = 0x05;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//开始捕获：	
	TIM_Cmd(TIM2, ENABLE);	// 开启时钟    
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// 清除溢出中断标志 
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
}

#else
/* 定时器2 通道2 (PA1) 输入 PPM 捕获配置 */
//TIM2_Capture_PPM_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
void TIM2_Capture_PPM_Init() // 捕获 遥控器 PPM 几通道
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定时器TIM2初始化   
	NVIC_InitTypeDef NVIC_InitStructure; 	//中断优先级NVIC设置
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOA时钟


	//gpio初始化：
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 

    TIM_DeInit(TIM2);//复位定时器2所有寄存器
	
	//初始化定时器2 TIM2	 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =72; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//输入捕获初始化：
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; //由于先检测低电平的同步头，所以这里下降沿触发
  	//下降沿触发
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM_ICInitStructure.TIM_ICFilter = 0x05;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//开始捕获：	
	TIM_Cmd(TIM2, ENABLE);	// 开启时钟    
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// 清除溢出中断标志 
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
}
#endif


/**/
u32 temp=0;
u8  TIM2_CH2_capture_state=0,ppm_rx_sta=0,ppm_rx_num=0;	//输入捕获状态		    				
u16	TIM2_CH2_capture_value;	//输入捕获值
u16 ppm_rx[10];//ppm_rx[0]   1   接收到ppm数据

//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM2中断
{
 	if( (TIM2_CH2_capture_state&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)		 
		{	    
			if(TIM2_CH2_capture_state&0X40)//已经捕获到高电平了
			{
				if((TIM2_CH2_capture_state&0X3F)==0X3F)//高电平太长了
				{
					TIM2_CH2_capture_state|=0X80;//标记成功捕获了一次
					TIM2_CH2_capture_value=0XFFFF;
				}
				else TIM2_CH2_capture_state++;
			}	 
		}
		if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM2_CH2_capture_state&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM2_CH2_capture_state|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM2_CH2_capture_value=TIM_GetCapture1(TIM2);
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}
			else  								//还未开始,第一次捕获上升沿
			{
				TIM2_CH2_capture_state=0;			//清空
				TIM2_CH2_capture_value=0;
				TIM_SetCounter(TIM2,0);
				TIM2_CH2_capture_state|=0X40;		//标记捕获到了上升沿
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
	
	/* 处理帧数据 */
	if( (TIM2_CH2_capture_state&0X80)==1)//成功捕获到了一次上升沿
	{
//			temp=TIM2_CH2_capture_state&0X3F;
//			temp*=65536;//溢出时间总和
//			temp+=TIM2_CH2_capture_value;//得到总的高电平时间	
		if(ppm_rx_sta==1) {ppm_rx[ppm_rx_num+1]=TIM2_CH2_capture_value;ppm_rx_num++;}//printf("TIM2_CH2_capture_value:%d\r\n",TIM2_CH2_capture_value);
		if(4>TIM2_CH2_capture_state&0X3F>0||TIM2_CH2_capture_value>3000) ppm_rx_sta++;//低电平时间大于3000us为起始帧
		if(ppm_rx_sta==2) {ppm_rx_sta=0;ppm_rx[0]=1;ppm_rx_num=0;}//printf("receive\r\n");//ppm_rx_sta   1 表示接收到同步帧/ 2接收到到下一起始帧 ppm数据接收完毕
		
		TIM2_CH2_capture_state=0;//开启下一次捕获
		
	}
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}



//----------------------------------------------------------------
// 舵机 摆动
/* 配置 TIM4 输出的 PWM 信号的模式，如周期、极性、占空比 */
void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	u16 CCR1_Val = 0;        
	u16 CCR2_Val = 0;
	u16 CCR3_Val = 0;
	u16 CCR4_Val = 0;

/* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;//定时器周期,20ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;//时钟预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数方式：向上计数
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

/* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式：tim脉宽调制2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//极性
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

/* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	
	
/* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	

/* PWM1 Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR4_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	

	/**/
    TIM_ARRPreloadConfig(TIM4, ENABLE);

    /* TIM1 enable counter */
    TIM_Cmd(TIM4, ENABLE);
    TIM_CtrlPWMOutputs(TIM4, ENABLE);	
}

void TIM4_Int_Init()/* 配置TIM1复用输出PWM时用到的I/O  */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//定时器1时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIOB时钟使能
	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*配置和初始化TIM1 */
void TIM4_PWM_SG90_MG90_Init(void)
{
	TIM4_Int_Init();
	TIM4_Mode_Config();	
}

	
//舵机摆动
void SG90_MG90_1(u16 num)//500-2500
{
	TIM4->CCR1 = num;//pwm波高电平持续时间，2ms
}
//舵机摆动
void SG90_MG90_2(u16 num)//500-2500
{
	TIM4->CCR2 = num;//pwm波高电平持续时间，2ms
}
//舵机摆动
void SG90_MG90_3(u16 num)//500-2500
{
	TIM4->CCR3 = num;//pwm波高电平持续时间，2ms
}
//舵机摆动
void SG90_MG90_4(u16 num)//500-2500
{
	TIM4->CCR4 = num;//pwm波高电平持续时间，2ms
}
