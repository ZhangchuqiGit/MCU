#ifndef __TFT_Drive_H_
#define	__TFT_Drive_H_

#include "stm32f10x.h"

/***************************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM4 连接 TFT，地址范围为0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A10 接LCD的DC(寄存器/数据选择)脚
寄存器基地址 = 0X6C00 0000
RAM基地址 = 0X6C00 0800  = 0X6C00 0000 + 0X0000 0800 = 0X6C00 0800
当选择不同的地址线时，地址要重新计算  
****************************************************************************************/

#define Bank1_LCD_C    ((u32)0x6C000000)	   //Disp Reg ADDR
#define Bank1_LCD_D    ((u32)0x6D000800)       //Disp Data ADDR       // A23 PE2

/*选定LCD指定寄存器*/
#define LCD_WR_REG(index)    ((*(__IO u16 *) (Bank1_LCD_C)) = ((u16)index))
/*往LCD GRAM写入数据*/
#define LCD_WR_Data(val)       ((*(__IO u16 *) (Bank1_LCD_D)) = ((u16)(val)))



#define TFT_XMAX		240
#define TFT_YMAX		400	


#define WHITE		 0xFFFF	/* 白色 */
#define BLACK        0x0000	/* 黑色 */
#define GREY         0xF7DE	/* 灰色 */
#define BLUE         0x001F	/* 蓝色 */
#define BLUE2        0x051F	/* 浅蓝色 */
#define RED          0xF800	/* 红色 */
#define MAGENTA      0xF81F	/* 红紫色，洋红色 */
#define GREEN        0x07E0	/* 绿色 */
#define CYAN         0x7FFF	/* 蓝绿色，青色 */
#define YELLOW       0xFFE0	/* 黄色 */

void TFT_GPIO_Config(void);
void TFT_FSMC_Config(void);
void TFT_Init(void);
void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void TFT_ClearScreen(uint16_t color);
void TFT_WriteData(uint16_t dat);
void TFT_WriteCmd(uint16_t cmd);

#endif
