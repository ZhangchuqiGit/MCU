#include"TFT_Drive.h"
/****************************************************************************
*函数名：TFT_GPIO_Config
*输  入：无
*输  出：无
*功  能：初始化TFT的IO口。
****************************************************************************/	  

void TFT_GPIO_Config(void)
{
	/*定义配置GPIO结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;	
	/* 打开时钟使能 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE  
	                      | RCC_APB2Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	/* FSMC_A10(G12) 和RS（G0）*/
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 
	                              | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_8 
								  | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11
								  | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
								  | GPIO_Pin_15 );

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
	                               | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
								   | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	GPIO_Init(GPIOE, &GPIO_InitStructure);
} 

void TFT_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    
    p.FSMC_AddressSetupTime = 0x02;	 //地址建立时间
    p.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    p.FSMC_DataSetupTime = 0x05;		 //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;
    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // 一般使用模式B来控制LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
//	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 
	 
 	/* 打开FSMC的时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);   
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* 使能 FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
  
}
/****************************************************************************
* Function Name  : TFT_WriteCmd
* Description    : LCD写入命令
* Input          : cmd：写入的16位命令
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteCmd(uint16_t cmd)
{
	LCD_WR_REG(cmd >> 8);
    LCD_WR_REG(cmd & 0x00FF);
}

/****************************************************************************
* Function Name  : TFT_WriteData
* Description    : LCD写入数据
* Input          : dat：写入的16位数据
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteData(uint16_t dat)
{
	LCD_WR_Data(dat >> 8);
    LCD_WR_Data(dat & 0x00FF);
}
/****************************************************************************
* Function Name  : TFT_Init
* Description    : 初始化LCD屏
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_Init(void)
{
	uint16_t i;

	TFT_GPIO_Config();
	TFT_FSMC_Config();

	for(i=500; i>0; i--);
	TFT_WriteCmd(0x0001);
	TFT_WriteData(0x0100);
	TFT_WriteCmd(0x0003);
	TFT_WriteData(0x1030); //16bit 565 
	TFT_WriteCmd(0x0008);
	TFT_WriteData(0x0808);
	TFT_WriteCmd(0x0090);
	TFT_WriteData(0x8000);
	TFT_WriteCmd(0x0400);
	TFT_WriteData(0x6200);
	TFT_WriteCmd(0x0401);
	TFT_WriteData(0x0001);

	TFT_WriteCmd(0x00ff);
	TFT_WriteData(0x0001);
	TFT_WriteCmd(0x0102);
	TFT_WriteData(0x01b0);
	TFT_WriteCmd(0x0710);
	TFT_WriteData(0x0016);
	TFT_WriteCmd(0x0712);
	TFT_WriteData(0x000f);
	TFT_WriteCmd(0x0752);
	TFT_WriteData(0x002f);
	TFT_WriteCmd(0x0724);
	TFT_WriteData(0x001a);
	TFT_WriteCmd(0x0754);	
	TFT_WriteData(0x0018);
	for(i=500; i>0; i--);
    
    /* Gamma */
	TFT_WriteCmd(0x0380);
	TFT_WriteData(0x0000);
	TFT_WriteCmd(0x0381);
	TFT_WriteData(0x5F10);//5f10
	TFT_WriteCmd(0x0382);
	TFT_WriteData(0x0B02);//0b02
	TFT_WriteCmd(0x0383);
	TFT_WriteData(0x0614);
	TFT_WriteCmd(0x0384);
	TFT_WriteData(0x0111);
	TFT_WriteCmd(0x0385);
	TFT_WriteData(0x0000);
	TFT_WriteCmd(0x0386);
	TFT_WriteData(0xA90B);//a90b
	TFT_WriteCmd(0x0387);
	TFT_WriteData(0x0606);//0606
	TFT_WriteCmd(0x0388);
	TFT_WriteData(0x0612);
	TFT_WriteCmd(0x0389);
	TFT_WriteData(0x0111);	  


    /* Gamma Setting */
    /* Setting */
	TFT_WriteCmd(0x0702);
	TFT_WriteData(0x003b);//003b
	TFT_WriteCmd(0x00ff);
	TFT_WriteData(0x0000);


    /* Vcom Setting */
	TFT_WriteCmd(0x0007);
	TFT_WriteData(0x0100);
	for(i=500; i>0; i--); 
	TFT_WriteCmd(0x0200);
	TFT_WriteData(0x0000);
	TFT_WriteCmd(0x0201);                  
	TFT_WriteData(0x0000);

}

/****************************************************************************
* Function Name  : TFT_SetWindow
* Description    : 设置要操作的窗口范围
* Input          : xStart：窗口起始X坐标
*                * yStart：窗口起始Y坐标
*                * xEnd：窗口结束X坐标
*                * yEnd：窗口结束Y坐标
* Output         : None
* Return         : None
****************************************************************************/

void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
 	TFT_WriteCmd(0x0210);   
    TFT_WriteData(xStart);
	TFT_WriteCmd(0x0211);  
     TFT_WriteData(xEnd);
	TFT_WriteCmd(0x0212);   
    TFT_WriteData(yStart);
	TFT_WriteCmd(0x0213);   
    TFT_WriteData(yEnd);

	TFT_WriteCmd(0x0200);   
    TFT_WriteData(xStart);
	TFT_WriteCmd(0x0201);   
    TFT_WriteData(yStart);

	TFT_WriteCmd(0x0202);
}











