#include"GUI.h"
#include"TFT_Drive.h"
#include"Picture.h"
#include"WordCHAR.h"
#include"CNChar.h"
/*********************************************************************************************
�� �� ����GUI_ClearScreen
�������ܣ�����
��ڲ�����color�Ǳ�����ɫ��
���ڲ���: ��
˵	  ����ʹ�ñ�����ɫ���TFTģ����Ļ��ȫ����ʾ���ݡ�
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
�� �� ����GUI_Point
�������ܣ�����
��ڲ�����color�ǵ����ɫ��
���ڲ���: ��
˵	  �������Ƕ��������ϻ��㡣
**********************************************************************************************/
void GUI_Point(u16 x,u16 y ,u16 Color)
{
	TFT_SetWindow(x,y,x,y);
	TFT_WriteData(Color);
}

/*********************************************************************************************
�� �� ����GUI_HLine
�������ܣ�������
��ڲ�����(x,y)��������꣬length���ߵĳ��ȣ�color���ߵ���ɫ��
���ڲ���: ��
˵	  ����ָ���ߵ���㼰���ȣ������ߡ�
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
�� �� ����GUI_RLine
�������ܣ�������
��ڲ�����(x,y)��������꣬high���ߵĳ��ȣ�color���ߵ���ɫ��
���ڲ���: ��
˵	  ����ָ���ߵ���㼰���ȣ������ߡ�
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
�� �� ����GUI_line
�������ܣ���ֱ�ߣ�����Ƕȵ�ֱ�ߡ�
��ڲ�����(x1,y1)��(x2,y2)�������յ����꣬��color���ߵ���ɫ��
���ڲ���: ��
˵	  ����ָ���ߵ���㼰�յ㣬��ֱ�ߡ�
**********************************************************************************************/
void GUI_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 Color)
{
 	u16 t;  
    int xerr=0,yerr=0,delta_x,delta_y,distance;  
    int incx,incy;  
    u16 row,col;  
    delta_x = x2-x1;//������������  
    delta_y = y2-y1;  
    col=x1;  
    row=y1;  
    if(delta_x>0) incx=1;//���õ�������  
    else   
    {  
        if(delta_x==0) incx=0;//��ֱ��  
        else {incx=-1;delta_x=(-delta_x);}  
    }  
    if(delta_y>0)incy=1;  
    else  
    {  
        if(delta_y==0) incy=0;//ˮƽ��  
        else {incy=-1;delta_y=(-delta_y);}  
    }  
    if(delta_x>delta_y) distance=delta_x;//ѡȡ��������������  
    else distance=delta_y;  

    for(t=0;t<=distance+1;t++)  
	{                                     //�������  
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
�� �� ����GUI_ShowPicture
�������ܣ���ʾͼƬ��
��ڲ�����(x,y)��ͼƬ������꣬length��ͼƬ�ĳ��ȣ�high��ͼƬ�ĸ߶ȡ�
���ڲ���: ��
˵	  ����ָ��ͼƬ��������꣬��ʾ����õ�ͼƬ��
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
		TFT_WriteData(temp);//�����ʾ
		tmp += 2;
	}
	while(tmp < num);	
}
/*********************************************************************************************
�� �� ����GUI_box
�������ܣ���ʵ�ľ���
��ڲ����� (sx,sy)���ϽǶ�������, (ex,ey)���½Ƕ�������, color��ɫ
���ڲ���: ��
˵	  ������ָ��λ���ϻ���ʵ�ľ��Ρ�
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
�� �� ����GUI_rectangle
�������ܣ������ľ���
��ڲ����� (sx,sy)���ϽǶ�������, (ex,ey)���½Ƕ�������, color��ɫ
���ڲ���: ��
˵	  ������ָ��λ���ϻ������ľ��Ρ�
*********************************************************************************************/
void GUI_rectangle( u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
	GUI_line(sx,sy,ex,sy,color);
	GUI_line(sx,sy,sx,ey,color);
	GUI_line(ex,sy,ex,ey,color);
	GUI_line(sx,ey,ex,ey,color);
}

/*********************************************************************************************
�� �� ����GUI_WriteCHAR
�������ܣ���ʾ�ַ���
��ڲ�����(x,y)���ַ�������꣬uchar *p���ַ����׵�ַ��uint wordColor��������ɫ��uint backColor������ɫ��
���ڲ���: ��
˵	  ����ָ���ַ���������꣬��ʾ�ַ���
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
�� �� ����GUI_WriteCHAR
�������ܣ���ʾ���֡�
��ڲ�����(x,y)���ַ�������꣬uchar *p���ַ����׵�ַ��uint wordColor��������ɫ��uint backColor������ɫ��
���ڲ���: ��
˵	  ����ָ���ַ���������꣬��ʾ�ַ���
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
