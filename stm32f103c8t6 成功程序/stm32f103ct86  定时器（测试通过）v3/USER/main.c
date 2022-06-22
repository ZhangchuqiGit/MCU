
/* * 实验平台：STM32F103C8T6  蓝色 */
/*STM32的单片机每个功能模块有自己的时钟系统,所以要想要调用STM32单片机的功能模块时 
必须先配置对应时钟,然后才能去操作相应的功能模块.端口重映射也一样.*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "misc.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
#include "GPIO_JTAG.h"


//**
int main(void)
{ 
    u8 len=0,t=0;
//    u8 sed[15]="play,001,$";
//    int music=0;
    SystemInit();//配置系统时钟为72M	
    LED_GPIO_Config();//led初始化

//    USART1_init(9600);
    USART2_init(9600);
    USART3_init(9600);

    //TIM2_Int_Init(1000-1,72-1);
    /**   1000-1    72-1  = 1ms   */ 
    /*中断周期为1ms*/
    //  TIM3_Int_Init(5000-1,7200-1);//10Khz的计数频率，计数到5000为500ms 
    // 中断/溢出  时间 t=  (  72M /(psc+1)  )  *  (  arr+1  )

    // EXTI0_Init();//总的中断初始化  A
    //EXTIX_Init(); //总的中断初始化  B

    // TIM1_PWM_Init();

    // printf(" -------电机测速------\r\n");

    PBout(8)=0; 
    PBout(9)=0; 
    while (1)
    {
        if(USART1_led)
        {
            USART1_led=0; 
            PBout(8)=1; 
            Delayms(100);
            PBout(8)=0; 
            Delayms(100);
            PBout(8)=1; 
            Delayms(100);
            PBout(8)=0; 
       }
        if(USART2_led)
        {
            PBout(9)=1; 
            Delayms(100);
            PBout(9)=0; 
            Delayms(100);
            PBout(9)=1; 
            Delayms(100);
            PBout(9)=0; 
            USART2_led=0; 
        }
    }

}

/*相较与其他的普通IO，PA13的Main function 为JTMS-SWDIO。反而普通IO口的功能在Alternate functions中的remap里。
也就是说PA13要想当做普通IO口使用，就必须使用它复用功能中的重映射。因此就需要这样的两步操作：

一.在时钟配置中打开复用时钟：  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
二.对PA13引脚进行重映射：        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); */

/* APB1 外设复位寄存器 (RCC_APB1RSTR)  
APB2 外设时钟使能寄存器(RCC_APB2ENR)
*/
/* 依照posix标准，一般整形相应的*_t类型为：
1字节     uint8_t
2字节     uint16_t
4字节     uint32_t
8字节     uint64_t*/

