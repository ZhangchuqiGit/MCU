/**
  ******************************************************************************
  * @file  stm32f10x_pwr.h
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  This file contains all the functions prototypes for the PWR firmware 
  *         library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_PWR_H
#define __STM32F10x_PWR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @addtogroup PWR
  * @{
  */ 

/** @defgroup PWR_Exported_Types
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup PWR_Exported_Constants
  * @{
  */ 

/** @defgroup PVD_detection_level 
  * @{
  */ 

#define PWR_PVDLevel_2V2          ((uint32_t)0x00000000)
#define PWR_PVDLevel_2V3          ((uint32_t)0x00000020)
#define PWR_PVDLevel_2V4          ((uint32_t)0x00000040)
#define PWR_PVDLevel_2V5          ((uint32_t)0x00000060)
#define PWR_PVDLevel_2V6          ((uint32_t)0x00000080)
#define PWR_PVDLevel_2V7          ((uint32_t)0x000000A0)
#define PWR_PVDLevel_2V8          ((uint32_t)0x000000C0)
#define PWR_PVDLevel_2V9          ((uint32_t)0x000000E0)
#define IS_PWR_PVD_LEVEL(LEVEL) (((LEVEL) == PWR_PVDLevel_2V2) || ((LEVEL) == PWR_PVDLevel_2V3)|| \
                                 ((LEVEL) == PWR_PVDLevel_2V4) || ((LEVEL) == PWR_PVDLevel_2V5)|| \
                                 ((LEVEL) == PWR_PVDLevel_2V6) || ((LEVEL) == PWR_PVDLevel_2V7)|| \
                                 ((LEVEL) == PWR_PVDLevel_2V8) || ((LEVEL) == PWR_PVDLevel_2V9))
/**
  * @}
  */

/** @defgroup Regulator_state_is_STOP_mode 
  * @{
  */

#define PWR_Regulator_ON  ((uint32_t)0x00000000)//电源不进低功耗 唤醒基本没延迟
#define PWR_Regulator_LowPower  ((uint32_t)0x00000001)//电源进去低功耗 不过唤醒启动有一点延迟    
#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_Regulator_ON) || \
                                     ((REGULATOR) == PWR_Regulator_LowPower))
/**
  * @}
  */

/** @defgroup STOP_mode_entry 
  * @{
  */

#define PWR_STOPEntry_WFI         ((uint8_t)0x01)
#define PWR_STOPEntry_WFE         ((uint8_t)0x02)
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPEntry_WFI) || ((ENTRY) == PWR_STOPEntry_WFE))
 
/**
  * @}
  */

/** @defgroup PWR_Flag 
  * @{
  */

#define PWR_FLAG_WU               ((uint32_t)0x00000001)
#define PWR_FLAG_SB               ((uint32_t)0x00000002)
#define PWR_FLAG_PVDO             ((uint32_t)0x00000004)
#define IS_PWR_GET_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB) || \
                               ((FLAG) == PWR_FLAG_PVDO))

#define IS_PWR_CLEAR_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup PWR_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup PWR_Exported_Functions
  * @{
  */
  
/*               
工作模式：外设正常运行，内核CPU及SRAM供电，未使用外设的时钟默认关闭。

睡眠模式：只有CPU停止工作，各个外设正常工作，依靠任何中断/事件唤醒。

停机模式：1.8V供电区域时钟被停止，内部HSI,PLL,外部时钟HSE均关闭，同时电压检测器也可进入低功耗模式，
但SRAM和寄存器不断电，此时只能依靠外部中断(EXTI)和RTC时钟才能唤醒。

待机模式：1.8V供电区域被时钟停止，内部HSI,PLL,外部时钟HSE均关闭，同时电压检测器也可进入低功耗模式，
SRAM和不属于待机电路的寄存器也被断电关闭，此时即使是外部中断(EXTI)也不能将其唤醒，
只能通过复位（外部复位，看门狗复位）、唤醒引脚、RTC时钟来唤醒。

注意：无论是在停止模式?还是待机模式下，RTC和IWDG及其时钟源不会被关闭                  */

void PWR_DeInit(void);
void PWR_BackupAccessCmd(FunctionalState NewState);
void PWR_PVDCmd(FunctionalState NewState);
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel);
void PWR_WakeUpPinCmd(FunctionalState NewState);//使能/唤醒管脚功能

FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void PWR_ClearFlag(uint32_t PWR_FLAG);

/*一般我们做开发大多都是选择*停机模式，因为停机模式功耗较低。
而且任一中断或事件都能唤醒，待机模式虽然功耗最低，也就差10个微安，
但是只有特定的唤醒脚可以唤醒，实时性不是很好。 
我们用停机模式也能降到10几个uA，实时性又好，干嘛不用呢。 */
/*我对比了 STM32F0 和 STM32F1 两者进入低功耗是一样的，STM32F4的类似目前没有研究。 
低功耗模式有三种， 
1.睡眠模式，（ CM3 内核停止，外设仍然运行）此功耗是最高的。 
2.停止模式，（所有时钟都停止）此功耗较低，典型大概在20uA左右。 
3.待机模式，（ 1.8V 内核电源关闭）此功耗最低，典型大概在2uA左右。 */

//void PWR_EnterSleepMode(uint8_t PWR_SLEEPEntry);//睡眠模式
void PWR_EnterSTANDBYMode(void);//待机模式
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);//停机模式
/*停机模式，有两个参数 
第一个参数 PWR_Regulator 是选择电源是否进入低功耗
#define PWR_Regulator_ON//电源不进低功耗 唤醒基本没延迟
#define PWR_Regulator_LowPower//电源进去低功耗 不过唤醒启动有一点延迟    
第二个参数 PWR_STOPEntry 
#define PWR_STOPEntry_WFI//中断唤醒
#define PWR_STOPEntry_WFE//事件唤醒  */ 

//PWR_EnterSTANDBYMode();//进入待机模式 唤醒延迟		
//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);//停机模式//电源不进低功耗 唤醒基本 没延迟//中断唤醒
//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFE);///停机模式//电源不进低功耗 唤醒基本 没延迟//事件唤醒
//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//停机模式//电源进去低功耗 不过唤醒启动 有一点延迟 //中断唤醒
//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);///停机模式//电源进去低功耗 不过唤醒启动 有一点延迟 //事件唤醒

#endif /* __STM32F10x_PWR_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
