#ifndef __TFT_Drive_H_
#define	__TFT_Drive_H_

#include "stm32f10x.h"

/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM4 ���� TFT����ַ��ΧΪ0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A10 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6C00 0000
RAM����ַ = 0X6C00 0800  = 0X6C00 0000 + 0X0000 0800 = 0X6C00 0800
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/

#define Bank1_LCD_C    ((u32)0x6C000000)	   //Disp Reg ADDR
#define Bank1_LCD_D    ((u32)0x6D000800)       //Disp Data ADDR       // A23 PE2

/*ѡ��LCDָ���Ĵ���*/
#define LCD_WR_REG(index)    ((*(__IO u16 *) (Bank1_LCD_C)) = ((u16)index))
/*��LCD GRAMд������*/
#define LCD_WR_Data(val)       ((*(__IO u16 *) (Bank1_LCD_D)) = ((u16)(val)))



#define TFT_XMAX		240
#define TFT_YMAX		400	


#define WHITE		 0xFFFF	/* ��ɫ */
#define BLACK        0x0000	/* ��ɫ */
#define GREY         0xF7DE	/* ��ɫ */
#define BLUE         0x001F	/* ��ɫ */
#define BLUE2        0x051F	/* ǳ��ɫ */
#define RED          0xF800	/* ��ɫ */
#define MAGENTA      0xF81F	/* ����ɫ�����ɫ */
#define GREEN        0x07E0	/* ��ɫ */
#define CYAN         0x7FFF	/* ����ɫ����ɫ */
#define YELLOW       0xFFE0	/* ��ɫ */

void TFT_GPIO_Config(void);
void TFT_FSMC_Config(void);
void TFT_Init(void);
void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void TFT_ClearScreen(uint16_t color);
void TFT_WriteData(uint16_t dat);
void TFT_WriteCmd(uint16_t cmd);

#endif
