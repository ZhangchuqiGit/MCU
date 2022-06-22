#ifndef __GUI_H_
#define __GUI_H_

#include"stm32f10x.h"


void GUI_ClearScreen(u16 color);
void GUI_Point(u16 x,u16 y ,u16 color);
void GUI_HLine(u16 x,u16 y,u16 length,u16 Color);
void GUI_RLine(u16 x,u16 y,u16 high,u16 Color);
void GUI_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 Color);
void GUI_ShowPicture(u16 x, u16 y, u16 wide, u16 high);
void GUI_WriteCHAR(u16 x, u16 y, u8 *p, u16 wordColor, u16 backColor);
void GUI_WriteCNChar(u16 x,u16 y,u8 *CN,u16 wordColor, u16 backColor);
void GUI_box(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void GUI_rectangle( u16 sx,u16 sy,u16 ex,u16 ey,u16 color);

#endif
