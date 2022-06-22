/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.  !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *     *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.         *
     *     *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:    *
     *    http://www.FreeRTOS.org/Documentation        *
     *     *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H


#include "sys.h"
#include "usart.h"


//针对不同的编译器调用不同的stdint.h文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    extern uint32_t SystemCoreClock;
#endif
/*  “config”开始的宏和“INCLUDE_”开始的宏一样，都是用来完成 FreeRTOS 的配置和裁剪的*/

/*在 V8.0.0 之前的 FreeRTOS 中会使用到这些数据类型，这些宏保证了你的代码从 V8.0.0 之
前 的 版 本 升 级 到 最 新 版 本 的 时 候 不 需 要 做 出 修 改 ， 默 认 情 况 下 宏
configENABLE_BACKWARD_COMPATIBILITY 为 1。*/
#define configENABLE_BACKWARD_COMPATIBILITY   1


/* *****************断言*********************** */
/*断言，类似 C 标准库中的 assert()函数， 调试代码的时候可以检查传入的参数是否合理，
FreeRTOS 内核中的关键点都会调用 configASSERT(x)， 当 x 为 0 的时候说明有错误发生，使用
断言的话会导致开销加大，一般在调试阶段使用*/
//  vAssertCalled()函数需要用户自行去定义，可以是显示到 LCD 上的函数，
// 也可以是通过串口打印出来的函数	
#define vAssertCalled(char,int) printf("FreeRTOSConfig_Error:%s,%d\r\n",char,int)
/*	当参数 x 错误的时候就通过串口打印出发生错误的文件名和错误所在的行号，
调试代码的可以使用断言，当调试完成以后尽量去掉断言， 防止增加开销  */
#define configASSERT(x) if((x)==0)vAssertCalled(__FILE__,__LINE__)

/***************************************************************************************************************/
/*                            FreeRTOS基础配置配置选项                              */
/***************************************************************************************************************/
#define configUSE_PREEMPTION					1 //1使用抢占式内核，0使用协程
#define configUSE_TIME_SLICING					1 //1使能时间片调度(默认式使能的)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1 //1启用特殊方法来选择下一个要运行的任务
     //一般是硬件计算前导零指令，如果所使用的
      //MCU没有这些硬件指令的话此宏应该设置为0！
#define configUSE_TICKLESS_IDLE					0 //1启用低功耗tickless模式
#define configUSE_QUEUE_SETS					1 //为1时启用队列
#define configCPU_CLOCK_HZ						(SystemCoreClock)       //CPU频率
//#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )	//CPU频率
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 ) //时钟节拍频率，这里设置为1000，周期就是1ms
		//此频率就是滴答定时器的中断频率，需要使用此宏来配置滴答定时器的中断，
		//	前面在讲 delay.c 文件的时候已经说过了。 本教程中我们将此宏设置为 1000，周期就是 1ms
#define configMAX_PRIORITIES				    (32)         //可使用的最大优先级
#define configMINIMAL_STACK_SIZE				((unsigned short)128)   //空闲任务使用的堆栈大小
#define configMAX_TASK_NAME_LEN					(16)         //任务名字字符串长度

#define configUSE_16_BIT_TICKS					0 //系统节拍计数器变量数据类型，
      //configUSE_16_BIT_TICKS : 1 表示TickType_t为16位无符号整形，0 表示TickType_t为32位无符号整形
#define configIDLE_SHOULD_YIELD					1 //为1时空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configUSE_TASK_NOTIFICATIONS			1 //为1时开启任务通知功能，默认开启
#define configUSE_MUTEXES						1 //为1时使用互斥信号量
#define configQUEUE_REGISTRY_SIZE				8   //不为0时表示启用队列记录，具体的值是可以
													//记录的队列和信号量最大数目。
#define configCHECK_FOR_STACK_OVERFLOW			0   //大于0时启用堆栈溢出检测功能，如果使用此功能
													//用户必须提供一个栈溢出钩子函数，如果使用的话
													//此值可以为1或者2，因为有两种栈溢出检测方法。
#define configUSE_RECURSIVE_MUTEXES				1 //为1时使用递归互斥信号量
#define configUSE_MALLOC_FAILED_HOOK			0 //1使用内存申请失败钩子函数
#define configUSE_APPLICATION_TASK_TAG			0 //为1的话函数configUSE_APPLICATION_TASK_TAGF()和xTaskCallApplicationTaskHook()就会被编译
#define configUSE_COUNTING_SEMAPHORES			1 //为1时使用计数信号量,相关的 API 函数会被编译

/***************************************************************************************************************/
/*          FreeRTOS与内存申请有关配置选项    */
/***************************************************************************************************************/
#define configSUPPORT_DYNAMIC_ALLOCATION        1 //支持动态内存申请
//#define configSUPPORT_STATIC_ALLOCATION       0 //支持静态内存申请
/*这里把堆空间设置为17K,堆空间都比 RAM (stm32f103c8t6 20KB , 但FLASH ROM有64KB)还大了，这程序怎么可能在单片机是正常运行，于是我把堆空格改为 10K，*/
#define configTOTAL_HEAP_SIZE					((size_t)(10*1024))     //系统所有总的堆大小
/***************************************************************************************************************/
/*          FreeRTOS与钩子函数有关的配置选项  */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK						0 //1，使用空闲钩子；0，不使用
#define configUSE_TICK_HOOK						0 //1，使用时间片钩子；0，不使用

/***************************************************************************************************************/
/*          FreeRTOS与运行时间和任务状态收集有关的配置选项           */
/***************************************************************************************************************/
#define configGENERATE_RUN_TIME_STATS	        0 //为1时启用运行时间统计功能
//#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()  ConfigureTimeForRunTimeStats()//定时器3提供时间统计的时基，频率为10K，即周期为100us
//#define portGET_RUN_TIME_COUNTER_VALUE()		  FreeRTOSRunTimeTicks	//获取时间统计时间值
#define configUSE_TRACE_FACILITY				0 //为1启用可视化跟踪调试
#define configUSE_STATS_FORMATTING_FUNCTIONS	1 //与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数 //prvWriteNameToBuffer(),vTaskList(),

/***************************************************************************************************************/
/*          FreeRTOS与协程有关的配置选项      */
/***************************************************************************************************************/
#define configUSE_CO_ROUTINES 			        0	//为1时启用协程，启用协程以后必须添加文件croutine.c
/*此宏为 1 的时候启用协程，协程可以节省开销，但是功能有限，现在的 MCU 性能已经非常强大了，建议关闭协程*/
#define configMAX_CO_ROUTINE_PRIORITIES       ( 2 )	//协程的有效优先级数目

/***************************************************************************************************************/
/*          FreeRTOS与软件定时器有关的配置选项           */
/***************************************************************************************************************/
#define configUSE_TIMERS				        1         //为1时启用软件定时器
#define configTIMER_TASK_PRIORITY		        (configMAX_PRIORITIES-1) //软件定时器任务的任务优先级
#define configTIMER_QUEUE_LENGTH		        5                  //软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH	        (configMINIMAL_STACK_SIZE*2) //软件定时器任务堆栈大小

/***************************************************************************************************************/
/*          FreeRTOS可选 函数配置选项          */
/***************************************************************************************************************/
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function.FreeRTOS可选函数配置选项   
使用“ INCLUDE_”开头的宏用来表示 使能或除能 FreeRTOS 中相应的 API 函数， 
作用就是用来配置 FreeRTOS 中的可选 API 函数的。
 如： INCLUDE_vTaskPrioritySet 表示使能函数vTaskPrioritySet()函数 */
#define INCLUDE_xTaskGetSchedulerState          1  
#define INCLUDE_vTaskPrioritySet		        1 //使能函数vTaskPrioritySet()函数
#define INCLUDE_uxTaskPriorityGet		        1
#define INCLUDE_vTaskDelete				        1 //使能函数vTaskDelete()函数
#define INCLUDE_vTaskCleanUpResources	        1 
#define INCLUDE_vTaskSuspend			        1
#define INCLUDE_vTaskDelayUntil			        1
#define INCLUDE_vTaskDelay				        1
#define INCLUDE_eTaskGetState			        1
#define INCLUDE_xTimerPendFunctionCall	        1
//#define INCLUDE_xTaskResumeFromISR   			1
//#define INCLUDE_xSemaphoreGetMutexHolder		1
//#define INCLUDE_xTaskAbortDelay				1

/***************************************************************************************************************/
/*          FreeRTOS与中断有关的配置选项      */
/***************************************************************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4       
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15           //中断最低优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	6 		//系统可管理的最高中断优先级
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
//      														(0x0f) 15	<<	(8  -  4)  =(0x0f0)=  240 
/*		configMAX_SYSCALL_INTERRUPT_PRIORITY 提供的可屏蔽中断优先级阈值是 191，对应的十六进制数是 0xBF
根据STM32F103 的优先级寄存器 的中断优先级的设计，只有高 4 位有效，还有FreeRTOS 默认将 4 个优先级位均划分为抢占优先级。
由于 FreeRTOS 官方提供的中断优先级阈值是 191（ 对应实际的 0xB） ， 也就是 11(0xB)~15 的优先级均可被操作系统屏蔽。
但我们实际使用时设置的中断优先级一般不会使用到 11 打后的，例如@正点原子的基础例程里面使用最多的 1~3，
 所以我们必须要修改这个值， 否则我们要重新修改所有底层驱动的优先级     */
//#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	0xb0 /* equivalent to 0xb0, or priority 11. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
//      														(0x06) 6	<<	(8  -  4)  =(0x060)=   96
///* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255(lowest)  to  0(1?)(highest). */
//#define configKERNEL_INTERRUPT_PRIORITY 		255
///* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
//See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */

/***************************************************************************************************************/
/*          FreeRTOS与中断服务函数有关的配置选项         */
/***************************************************************************************************************/
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler
#define vPortSysTickHandler SysTick_Handler
//void NMI_Handler(void);
//void HardFault_Handler(void);
//void MemManage_Handler(void);
//void BusFault_Handler(void);
//void UsageFault_Handler(void);
//void SVC_Handler(void);
//void DebugMon_Handler(void);
//void PendSV_Handler(void);

/* This is the value being used as per the ST library which permits 16 priority values,
 0 to 15.  This must correspond to the configKERNEL_INTERRUPT_PRIORITY setting.   
 Here 15 corresponds to the lowest NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

#endif /* FREERTOS_CONFIG_H */

/*

FreeRTOS使用一个名为FreeRTOSConfig.h的配置文件进行自定义。
每个FreeRTOS应用程序必须在其预处理器包含路径中有一个FreeRTOSConfig.h头文件。
h将RTOS内核裁剪为正在构建的应用程序。因此，它是特定于应用程序的，而不是RTOS，并且应该位于应用程序目录中，
而不是在RTOS内核源代码目录中。RTOS源代码下载中包含的每个演示应用程序都有自己的FreeRTOSConfig.h文件。
有些演示程序非常陈旧，并不包含所有可用的配置选项。忽略的配置选项设置为RTOS源文件中的默认值。

下面是一个典型的FreeRTOSConfig.h定义，然后是对每个参数的解释

https://www.freertos.org/a00110.html


*/

