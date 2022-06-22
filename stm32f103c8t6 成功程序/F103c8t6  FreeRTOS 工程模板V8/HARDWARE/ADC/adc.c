#include "adc.h"

/* 描述    ：DMA方式读取ADC值应用函数库         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接： PCA0 - ADC1 */
#if DMA_ADC_EN

#define ADC1_DR_Address    ((u32)0x4001244C)

__IO u16 ADC_ConvertedValue[4];/*设置DMA内存地址，ADC转换结果直接放入该地址*/

/*******************初始化ADC1++++++++++++++++++++*/
//模拟输入端为PA 0 1 2 3/*配置ADC1的工作模式为MDA模式  */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	DMA_Mode_Config();
	ADC_Mode_Config();

}
/*配置采样通道端口 使能GPIO时钟	  设置ADC采样PA0端口信号*/
 void ADC1_GPIO_Config(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;    
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	  
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;//PA 0 1 2 3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
//    GPIO_Init(GPIOA,&GPIO_InitStructure);      // 初始化GPIOB
    GPIO_Init(GPIOB,&GPIO_InitStructure);      // 初始化GPIOB
}
 void ADC_Mode_Config(void)
 {
    ADC_InitTypeDef ADC_InitStructure;	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//使能ADC1时钟

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //使用独立模式，扫描模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //无需外接触发器
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //使用数据右对齐
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//    ADC_InitStructure.ADC_NbrOfChannel =4;  // ADC转换通道个数
    ADC_InitStructure.ADC_NbrOfChannel =2;  // ADC转换通道个数
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel11 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);//通道1采样周期55.5个时钟周期 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 4, ADC_SampleTime_239Cycles5);//这一通道采集芯片内部温度

//    /* ADC1 regular channel11 configuration */ 
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
//     ////ADC1选择信道0,转换顺序1，采样时间55.5周期         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); 
//     ////ADC1选择信道1,转换顺序2，采样时间55.5周期         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
//      ////ADC1选择信道2,转换顺序3，采样时间55.5周期         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
//     ////ADC1选择信道3,转换顺序4，采样时间55.5周期         
//*-+
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	 //使能ADC的DMA
//*-+
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE); //使能ADC1
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));

    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //开始转换
 }
/*配置ADC1的工作模式为MDA模式  */
 void DMA_Mode_Config(void)
{
    //++++++++++++++++**************------------//////////////
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能MDA1时钟
    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);  //指定DMA通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;////ADC1地址---代表ADC1保存转换值的寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	/*设置DMA内存地址，ADC转换结果直接放入该地址*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
    DMA_InitStructure.DMA_BufferSize = 4;	//传输总数据---2通道需要传输2个数据
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址固定
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增---总体表示始终从外设ADC1地址处取值---依次保存到连续的两个内存变量中---
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //内存传输数据单元---半字16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式---2个数据依次循环接收从外设ADC1传输过来的ADC值---
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA channel1 */
    //+++&^%$#**-/-*
    DMA_Cmd(DMA1_Channel1, ENABLE);  //使能DMA通道
//*-+
//*-+   ADC_DMACmd(ADC1, ENABLE);	 //使能ADC的DMA

}

////    AD1_value  = 3300000/4096*ADC_ConvertedValue[0]/1000;//PA 0 
////    AD2_value  = 3300000/4096*ADC_ConvertedValue[1]/1000;//PA 1
////    AD3_value  = 3300000/4096*ADC_ConvertedValue[2]/1000;//PA 2
////    AD4_value  = 3300000/4096*ADC_ConvertedValue[3]/1000;//PA 3
////     printf("AD1 value = %d mV  \r\n", AD1_value);
////     printf("AD2 value = %d mV  \r\n", AD2_value);
////     printf("AD3 value = %d mV  \r\n", AD3_value);
////     printf("AD4 value = %d mV  \r\n", AD4_value);

#endif

void ADC1_Mode_init(void)///stm32_adc转换，模拟输入端为PB0
{
    /* ADC1 configuration */
    ADC_InitTypeDef   ADC_InitStructure;  //声明一个结构体
    GPIO_InitTypeDef GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC1时钟
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //PB0设为模拟输入脚
    GPIO_Init(GPIOB,&GPIO_InitStructure);  // 初始化GPIOB

    RCC_ADCCLKConfig(RCC_PCLK2_Div4);   //配置ADC时钟分频
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//使用独立模式，扫描模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //使能多通道扫描模式//模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换模式//无需外接触发器
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //软件启动转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;  //指定将转换的ADC通道的数量
    ADC_Init(ADC1, &ADC_InitStructure);   //初始化ADC
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5); 
//ADC1选择信道8,转换顺序1，采样时间55.5周期         
    ADC_Cmd(ADC1, ENABLE); //使能ADC1
    ADC_ResetCalibration(ADC1);       //重置ADC1校准寄存器
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待ADC1校准重置完成
    ADC_StartCalibration(ADC1); //开始ADC1校准
    while(ADC_GetCalibrationStatus(ADC1));  //等待ADC1校准完成
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能ADC1软件开始转换

}
