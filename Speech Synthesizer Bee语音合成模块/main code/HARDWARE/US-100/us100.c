#include "us100.h" 
/*
引脚连接：TRIG--PC15   ECHO--PC13
*/

void hcsr04Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    EXTI_InitTypeDef EXTI_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;         
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);  // 中断线以及中断初始化配置

    EXTI_ClearITPendingBit(EXTI_Line1);

    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);     

    /* Enable TIM4 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* TIME4 base configuration */
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;                   //  
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;                  // 设置预分频,F=72MHz/72=1MHz,T=1us
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;                 // 设置时钟分频系数,不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);              // 初始化TIME5

    TIM_Cmd(TIM4, DISABLE); 
}


/**
 * 发出测试信号
 */
void hcsr04StartRanging(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_15);
    Delay_us(20);   //  The width of trig signal must be greater than 10us
    GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}

/**
 * 根据公式计算距离 
 * @return distance units:cm 
 */
float hcsr04GetDistance(void)
{
    // distance = measurement/2/1000*340 = measurement/59 (cm)  measurement-units:us
    float distance = measurement / 58.8;   // measurement-units:us

    return distance;
}

static void ECHO_EXTI_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) != 0) {  // 等待高电平回波
            TIM_Cmd(TIM4, ENABLE);  
        } else {
            TIM_Cmd(TIM4, DISABLE);  
            measurement = TIM_GetCounter(TIM4);
            TIM_SetCounter(TIM4, 0); 
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI1_IRQHandler(void)
{
    ECHO_EXTI_IRQHandler();
}
