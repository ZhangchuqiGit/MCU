#include"GUI.h"
#include"TFT_Drive.h"
#include"Picture.h"
#include"WordCHAR.h"
#include"CNChar.h"
/*********************************************************************************************
函 数 名：GUI_ClearScreen
函数功能：清屏
入口参数：color是背景颜色。
出口参数: 无
说	  明：使用背景颜色清除TFT模块屏幕的全部显示内容。
**********************************************************************************************/
void GUI_ClearScreen(u16 Color)
{
	u16 i,j;
	TFT_SetWindow(0,0,TFT_XMAX,TFT_YMAX);
	for(i=0;i<TFT_YMAX;i++)
	{
		for(j=0;j<TFT_XMAX+1;j++)
			TFT_WriteData(Color);
	}				
}

/*********************************************************************************************
函 数 名：GUI_Point
函数功能：画点
入口参数：color是点的颜色。
出口参数: 无
说	  明：在是定的坐标上画点。
**********************************************************************************************/
void GUI_Point(u16 x,u16 y ,u16 Color)
{
	TFT_SetWindow(x,y,x,y);
	TFT_WriteData(Color);
}

/*********************************************************************************************
函 数 名：GUI_HLine
函数功能：画横线
入口参数：(x,y)是起点坐标，length是线的长度，color是线的颜色。
出口参数: 无
说	  明：指定线的起点及长度，画横线。
**********************************************************************************************/
void GUI_HLine(u16 x,u16 y,u16 length,u16 Color)
{
	u16 EndX,Count;
	if((length+x-1)>TFT_XMAX)
	{
		EndX=TFT_XMAX;
		Count=TFT_XMAX-x;
	}
	else
	{
		EndX=length+x-1;
		Count=length;
	}		
	
	TFT_SetWindow(x,y,EndX,y);	
	while(Count)
	{
	 	Count--;
		TFT_WriteData(Color);
	}
}

/*********************************************************************************************
函 数 名：GUI_RLine
函数功能：画竖线
入口参数：(x,y)是起点坐标，high是线的长度，color是线的颜色。
出口参数: 无
说	  明：指定线的起点及长度，画竖线。
**********************************************************************************************/
void GUI_RLine(u16 x,u16 y,u16 high,u16 Color)
{
	u16 EndY,Count;
	if((high+y-1)>TFT_YMAX)
	{
		EndY=TFT_YMAX;
		Count=TFT_YMAX-y;
	}
	else
	{
		EndY=high+y-1;
		Count=high;
	}			
	
	TFT_SetWindow(x,y,x,EndY);
	while(Count)
	{
	 	Count--;
		TFT_WriteData(Color);
	}
}

/*********************************************************************************************
函 数 名：GUI_line
函数功能：画直线，任意角度的直线。
入口参数：(x1,y1)，(x2,y2)是起点和终点坐标，，color是线的颜色。
出口参数: 无
说	  明：指定线的起点及终点，画直线。
**********************************************************************************************/
void GUI_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 Color)
{
 	u16 t;  
    int xerr=0,yerr=0,delta_x,delta_y,distance;  
    int incx,incy;  
    u16 row,col;  
    delta_x = x2-x1;//计算坐标增量  
    delta_y = y2-y1;  
    col=x1;  
    row=y1;  
    if(delta_x>0) incx=1;//设置单步方向  
    else   
    {  
        if(delta_x==0) incx=0;//垂直线  
        else {incx=-1;delta_x=(-delta_x);}  
    }  
    if(delta_y>0)incy=1;  
    else  
    {  
        if(delta_y==0) incy=0;//水平线  
        else {incy=-1;delta_y=(-delta_y);}  
    }  
    if(delta_x>delta_y) distance=delta_x;//选取基本增量坐标轴  
    else distance=delta_y;  

    for(t=0;t<=distance+1;t++)  
	{                                     //画线输出  
	    GUI_Point(col, row, Color);
        xerr+=delta_x;  
        yerr+=delta_y;  
      	if(xerr>distance)  
        {  
            xerr-=distance;  
            col+=incx;  
        }  
        if(yerr>distance)  
        {  
            yerr-=distance;  
            row+=incy;  
        }  
    }  
}

/*********************************************************************************************
函 数 名：GUI_ShowPicture
函数功能：显示图片。
入口参数：(x,y)是图片起点坐标，length是图片的长度，high是图片的高度。
出口参数: 无
说	  明：指定图片的起点坐标，显示定义好的图片。
**********************************************************************************************/
void GUI_ShowPicture(u16 x, u16 y, u16 wide, u16 high)
{
	u16 temp = 0, tmp = 0, num = 0;
	TFT_SetWindow(x, y, x+wide-1, y+high-1);
	num = wide * high * 2;
	do
	{  
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		TFT_WriteData(temp);//逐点显示
		tmp += 2;
	}
	while(tmp < num);	
}
/*********************************************************************************************
函 数 名：GUI_box
函数功能：画实心矩形
入口参数： (sx,sy)左上角顶点坐标, (ex,ey)右下角顶点坐标, color颜色
出口参数: 无
说	  明：在指定位置上画出实心矩形。
*********************************************************************************************/
void GUI_box(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{ 
	u16 temp;   
	TFT_SetWindow(sx,sy,ex,ey); 
    sx=ex-sx+1;
	sy=ey-sy+1;
	temp=sx*sy;
	while(temp)
	{
	 TFT_WriteData(color);		
	 temp--;		
	}
}
/*********************************************************************************************
函 数 名：GUI_rectangle
函数功能：画空心矩形
入口参数： (sx,sy)左上角顶点坐标, (ex,ey)右下角顶点坐标, color颜色
出口参数: 无
说	  明：在指定位置上画出空心矩形。
*********************************************************************************************/
void GUI_rectangle( u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
	GUI_line(sx,sy,ex,sy,color);
	GUI_line(sx,sy,sx,ey,color);
	GUI_line(ex,sy,ex,ey,color);
	GUI_line(sx,ey,ex,ey,color);
}

/*********************************************************************************************
函 数 名：GUI_WriteCHAR
函数功能：显示字符。
入口参数：(x,y)是字符起点坐标，uchar *p是字符串首地址，uint wordColor是字体颜色，uint backColor背景颜色。
出口参数: 无
说	  明：指定字符的起点坐标，显示字符。
**********************************************************************************************/
void GUI_WriteCHAR(u16 x, u16 y, u8 *p, u16 wordColor, u16 backColor)
{
	u8 WordNum,WordByte,WordBit,i;
	while(*p != '\0')
	{
		TFT_SetWindow(x,y,x+7,y+15);
		WordNum=*p-32;
		for(WordByte=0;WordByte<16;WordByte++)
		{
		   WordBit=	WordCHAR[WordNum][WordByte];
		   for(i=0;i<8;i++)
		   {
				if(WordBit&0x80)
				{
				TFT_WriteData(wordColor);	
				}
				else
				{
				TFT_WriteData(backColor);
				}
				WordBit <<= 1;
		   }
		}
		p++;
		x=x+8;
		if(x>TFT_XMAX||((TFT_XMAX-x)<8))
		{
			x=0;
			y+=16;
		}		
	}
}


/*********************************************************************************************
函 数 名：GUI_WriteCHAR
函数功能：显示汉字。
入口参数：(x,y)是字符起点坐标，uchar *p是字符串首地址，uint wordColor是字体颜色，uint backColor背景颜色。
出口参数: 无
说	  明：指定字符的起点坐标，显示字符。
**********************************************************************************************/
void GUI_WriteCNChar(u16 x,u16 y,u8 *CN,u16 wordColor, u16 backColor)
{
	u8 WordNum,WordBit,i,j;
	while(*CN != '\0')
	{
		TFT_SetWindow(x,y,x+31,y+28);
		for(WordNum=0;WordNum<20;WordNum++)
		{
			if((*CN==CNChar[WordNum].Index[0])&&(*(CN+1)==CNChar[WordNum].Index[1]))
			{
				for(i=0;i<116;i++)
				{
				 	WordBit=CNChar[WordNum].Msk[i];
					for(j=0;j<8;j++)
					{
					 	if(WordBit&0x80)
						{
							TFT_WriteData(wordColor);
						}					
						else
						{
							TFT_WriteData(backColor);
						}
						WordBit <<= 1;					
					}
				}			  
			}
		}
		CN += 2;
		x += 32;
		if(x>TFT_XMAX||((TFT_XMAX-x)<32))
		{
			x=0;
			y+=29;
		}
	}		
}
