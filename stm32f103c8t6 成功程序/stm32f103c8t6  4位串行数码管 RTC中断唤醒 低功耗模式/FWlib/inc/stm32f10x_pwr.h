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

#define PWR_Regulator_ON  ((uint32_t)0x00000000)//��Դ�����͹��� ���ѻ���û�ӳ�
#define PWR_Regulator_LowPower  ((uint32_t)0x00000001)//��Դ��ȥ�͹��� ��������������һ���ӳ�    
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
����ģʽ�������������У��ں�CPU��SRAM���磬δʹ�������ʱ��Ĭ�Ϲرա�

˯��ģʽ��ֻ��CPUֹͣ�����������������������������κ��ж�/�¼����ѡ�

ͣ��ģʽ��1.8V��������ʱ�ӱ�ֹͣ���ڲ�HSI,PLL,�ⲿʱ��HSE���رգ�ͬʱ��ѹ�����Ҳ�ɽ���͹���ģʽ��
��SRAM�ͼĴ������ϵ磬��ʱֻ�������ⲿ�ж�(EXTI)��RTCʱ�Ӳ��ܻ��ѡ�

����ģʽ��1.8V��������ʱ��ֹͣ���ڲ�HSI,PLL,�ⲿʱ��HSE���رգ�ͬʱ��ѹ�����Ҳ�ɽ���͹���ģʽ��
SRAM�Ͳ����ڴ�����·�ļĴ���Ҳ���ϵ�رգ���ʱ��ʹ���ⲿ�ж�(EXTI)Ҳ���ܽ��份�ѣ�
ֻ��ͨ����λ���ⲿ��λ�����Ź���λ�����������š�RTCʱ�������ѡ�

ע�⣺��������ֹͣģʽ?���Ǵ���ģʽ�£�RTC��IWDG����ʱ��Դ���ᱻ�ر�                  */

void PWR_DeInit(void);
void PWR_BackupAccessCmd(FunctionalState NewState);
void PWR_PVDCmd(FunctionalState NewState);
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel);
void PWR_WakeUpPinCmd(FunctionalState NewState);//ʹ��/���ѹܽŹ���

FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void PWR_ClearFlag(uint32_t PWR_FLAG);

/*һ��������������඼��ѡ��*ͣ��ģʽ����Ϊͣ��ģʽ���Ľϵ͡�
������һ�жϻ��¼����ܻ��ѣ�����ģʽ��Ȼ������ͣ�Ҳ�Ͳ�10��΢����
����ֻ���ض��Ļ��ѽſ��Ի��ѣ�ʵʱ�Բ��Ǻܺá� 
������ͣ��ģʽҲ�ܽ���10����uA��ʵʱ���ֺã����ﲻ���ء� */
/*�ҶԱ��� STM32F0 �� STM32F1 ���߽���͹�����һ���ģ�STM32F4������Ŀǰû���о��� 
�͹���ģʽ�����֣� 
1.˯��ģʽ���� CM3 �ں�ֹͣ��������Ȼ���У��˹�������ߵġ� 
2.ֹͣģʽ��������ʱ�Ӷ�ֹͣ���˹��Ľϵͣ����ʹ����20uA���ҡ� 
3.����ģʽ���� 1.8V �ں˵�Դ�رգ��˹�����ͣ����ʹ����2uA���ҡ� */

//void PWR_EnterSleepMode(uint8_t PWR_SLEEPEntry);//˯��ģʽ
void PWR_EnterSTANDBYMode(void);//����ģʽ
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);//ͣ��ģʽ
/*ͣ��ģʽ������������ 
��һ������ PWR_Regulator ��ѡ���Դ�Ƿ����͹���
#define PWR_Regulator_ON//��Դ�����͹��� ���ѻ���û�ӳ�
#define PWR_Regulator_LowPower//��Դ��ȥ�͹��� ��������������һ���ӳ�    
�ڶ������� PWR_STOPEntry 
#define PWR_STOPEntry_WFI//�жϻ���
#define PWR_STOPEntry_WFE//�¼�����  */ 

//PWR_EnterSTANDBYMode();//�������ģʽ �����ӳ�		
//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);//ͣ��ģʽ//��Դ�����͹��� ���ѻ��� û�ӳ�//�жϻ���
//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFE);///ͣ��ģʽ//��Դ�����͹��� ���ѻ��� û�ӳ�//�¼�����
//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//ͣ��ģʽ//��Դ��ȥ�͹��� ������������ ��һ���ӳ� //�жϻ���
//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);///ͣ��ģʽ//��Դ��ȥ�͹��� ������������ ��һ���ӳ� //�¼�����

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
