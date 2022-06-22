#ifndef __LCD_H
#define __LCD_H		
#include "stdlib.h"
#include "sys.h"	 
#include "usart.h"	 
#include "delay.h"	 

#include "picture.h"
#include "font.h" 

//支持驱动IC型号包括:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963等		    


//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_lcd_dev; 	  //_tftlcd_data//tftlcd_data

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD端口定义---------------- 
#define	LCD_LED PBout(0) //LCD背光  PB0 	    
//LCD地址结构体
typedef struct
{
	vu16 LCD_REG;//LCD_CMD;
	vu16 LCD_RAM;//LCD_DATA;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	
//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)
	
void LCD_Init(void); //初始化
void LCD_DisplayOn(void); //开显示
void LCD_DisplayOff(void); //关显示
void LCD_Clear(u16 Color); //清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos); //设置光标
void LCD_DrawPoint(u16 x,u16 y); //画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color); //快速画点
u16  LCD_ReadPoint(u16 x,u16 y); //读点 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r); //画圆环
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2); //画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2); //画矩形
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color); //填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color); //填充指定颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode); //显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size); //显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode); //显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p); //显示一个字符串,12/16字体

void LCD_WR_REG(u16 regval);//写寄存器//LCD_WriteCmd
void LCD_WR_DATA(u16 data);//写LCD数据//LCD_WriteData
u16 LCD_RD_DATA(void);//读LCD数据
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue);//写寄存器
u16 LCD_ReadReg(u16 LCD_Reg);//读寄存器

void LCD_WriteRAM_Prepare(void);//开始写GRAM
void LCD_WriteRAM(u16 RGB_Code);//RGB_Code:颜色值
void LCD_SSD_BackLightSet(u8 pwm);//SSD1963 背光控制
void LCD_Scan_Dir(u8 dir);//设置屏扫描方向

void LCD_Display_Dir(u8 dir);//设置屏幕显示方向
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);//设置窗口					   						   																			 

//------------------------------------------------------------
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);//画水平线
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);//画实心圆
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);//画一条粗线

//显示程序：图像显示函数,适用Image2Lcd V2.9 图像编译软件  By:gaofei                
void Picture_Draw(u16 S_x,u16 S_y,const unsigned char *pic);//在指定座标范围显示一副图片
void LCD_Draw_Picture(u16 xstr,u16 ystr,u16 xend,u16 yend,u16 color_user,u8 *pic);//在指定位置显示一幅单色图
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr);//在指定座标显示一串字符透明叠加在背景图片上


#endif  	
	 /*
1，写寄存器值函数 ：void LCD_WR_REG(u16 regval)
2，写数据函数：void LCD_WR_DATA(u16 data)
3，读数据函数：u16 LCD_RD_DATA(void)
4，写寄存器内容函数： void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
5，读寄存器内容函数： u16 LCD_ReadReg(u16 LCD_Reg)
6，开始写GRAM函数： void LCD_WriteRAM_Prepare(void)
7，写GRAM函数： void LCD_WriteRAM(u16 RGB_Code)
*/


