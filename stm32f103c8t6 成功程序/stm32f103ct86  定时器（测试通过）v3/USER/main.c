
/* * ʵ��ƽ̨��STM32F103C8T6  ��ɫ */
/*STM32�ĵ�Ƭ��ÿ������ģ�����Լ���ʱ��ϵͳ,����Ҫ��Ҫ����STM32��Ƭ���Ĺ���ģ��ʱ 
���������ö�Ӧʱ��,Ȼ�����ȥ������Ӧ�Ĺ���ģ��.�˿���ӳ��Ҳһ��.*/
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
    SystemInit();//����ϵͳʱ��Ϊ72M	
    LED_GPIO_Config();//led��ʼ��

//    USART1_init(9600);
    USART2_init(9600);
    USART3_init(9600);

    //TIM2_Int_Init(1000-1,72-1);
    /**   1000-1    72-1  = 1ms   */ 
    /*�ж�����Ϊ1ms*/
    //  TIM3_Int_Init(5000-1,7200-1);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
    // �ж�/���  ʱ�� t=  (  72M /(psc+1)  )  *  (  arr+1  )

    // EXTI0_Init();//�ܵ��жϳ�ʼ��  A
    //EXTIX_Init(); //�ܵ��жϳ�ʼ��  B

    // TIM1_PWM_Init();

    // printf(" -------�������------\r\n");

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

/*�������������ͨIO��PA13��Main function ΪJTMS-SWDIO��������ͨIO�ڵĹ�����Alternate functions�е�remap�
Ҳ����˵PA13Ҫ�뵱����ͨIO��ʹ�ã��ͱ���ʹ�������ù����е���ӳ�䡣��˾���Ҫ����������������

һ.��ʱ�������д򿪸���ʱ�ӣ�  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
��.��PA13���Ž�����ӳ�䣺        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); */

/* APB1 ���踴λ�Ĵ��� (RCC_APB1RSTR)  
APB2 ����ʱ��ʹ�ܼĴ���(RCC_APB2ENR)
*/
/* ����posix��׼��һ��������Ӧ��*_t����Ϊ��
1�ֽ�     uint8_t
2�ֽ�     uint16_t
4�ֽ�     uint32_t
8�ֽ�     uint64_t*/

