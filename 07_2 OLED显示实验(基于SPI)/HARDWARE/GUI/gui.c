#include "gui.h"
#include "oledfont.h" 

/*******************************************************************
 * @name       :void GUI_DrawPoint(uint8_t x,uint8_t y,uint8_t color)
 * @function   :在屏幕上画一个点
 * @parameters :x:点的x坐标
                y:点的y坐标
				color:点的颜色值 1-白色 0-黑色
 * @retvalue   :无
********************************************************************/
void GUI_DrawPoint(uint8_t x,uint8_t y,uint8_t color)
{
	OLED_Set_Pixel(x,y,color);
	OLED_Display();
}


/*******************************************************************
 * @name       :void GUI_Fill(uint8_t sx,uint8_t sy,uint8_t ex,uint8_t ey,uint8_t color)
 * @function   :填充指定区域
 * @parameters :sx:指定区域的起始x坐标
                sy:指定区域的起始y坐标
				ex:指定区域的结束x坐标
				ey:指定区域的结束y坐标
				color:指定区域的颜色值 1-白色 0-黑色
 * @retvalue   :无
********************************************************************/
void GUI_Fill(uint8_t sx,uint8_t sy,uint8_t ex,uint8_t ey,uint8_t color)
{  	
	uint8_t i,j;			
	uint8_t width=ex-sx+1; 		//得到填充的宽度
	uint8_t height=ey-sy+1;		//高度
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
				OLED_Set_Pixel(sx+j, sy+i,color);
		}		
	}
	OLED_Display();
}


/*******************************************************************
 * @name       :void GUI_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color)
 * @function   :在两点之间画一条线
 * @parameters :x1:直线的起始x坐标
                y1:直线的起始y坐标
				x2:直线的终点x坐标
				y2:直线的终点y坐标
				color:线条的颜色值 1-白色 0-黑色
 * @retvalue   :无
********************************************************************/
void GUI_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{ 
		OLED_Set_Pixel(uRow,uCol,color);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
	OLED_Display();
} 


/*****************************************************************************
 * @name       :void GUI_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color)
 * @function   :画个矩形
 * @parameters :x1:矩形的起始x坐标
                y1:矩形的起始y坐标
				x2:矩形的结束x坐标
				y2:矩形的结束y坐标
				color:矩形的颜色值 1-白色 0-黑色				  
 * @retvalue   :无
******************************************************************************/
void GUI_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color)
{
	GUI_DrawLine(x1,y1,x2,y1,color);
	GUI_DrawLine(x1,y1,x1,y2,color);
	GUI_DrawLine(x1,y2,x2,y2,color);
	GUI_DrawLine(x2,y1,x2,y2,color);
}  


/*****************************************************************************
 * @name       :void GUI_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color)
 * @function   :填充矩形
 * @parameters :x1:矩形的起始x坐标
                y1:矩形的起始y坐标
				x2:矩形的结束x坐标
				y2:矩形的结束y坐标
				color:矩形的颜色值 1-白色 0-黑色
 * @retvalue   :无
******************************************************************************/  
void GUI_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color)
{
	GUI_Fill(x1,y1,x2,y2,color);
}


/*****************************************************************************
 * @name       :static void _draw_circle_8(uint8_t xc, uint8_t yc, uint8_t x, uint8_t y, uint8_t color)
 * @function   :对称圆绘制算法(内部调用)
 * @parameters :xc:圆心的x坐标 
                yc:圆心的y坐标
				x:相对于圆心的x坐标
				y:相对于圆心的y坐标 
				color:圆的颜色值 1-白色 0-黑色
 * @retvalue   :无
******************************************************************************/  
static void _draw_circle_8(uint8_t xc, uint8_t yc, uint8_t x, uint8_t y, uint8_t color)
{
	OLED_Set_Pixel(xc + x, yc + y, color);
	OLED_Set_Pixel(xc - x, yc + y, color);
	OLED_Set_Pixel(xc + x, yc - y, color);
	OLED_Set_Pixel(xc - x, yc - y, color);
	OLED_Set_Pixel(xc + y, yc + x, color);
	OLED_Set_Pixel(xc - y, yc + x, color);
	OLED_Set_Pixel(xc + y, yc - x, color);
	OLED_Set_Pixel(xc - y, yc - x, color);
}


/*****************************************************************************
 * @name       :void GUI_DrawCircle(uint8_t xc, uint8_t yc, uint8_t color, uint8_t r)
 * @function   :在指定位置绘制指定大小的圆
 * @parameters :xc:圆心的x坐标
                yc:圆心的y坐标
				r:半径
				color:圆的颜色值 1-白色 0-黑色
 * @retvalue   :无
******************************************************************************/  
void GUI_DrawCircle(uint8_t xc, uint8_t yc, uint8_t color, uint8_t r)
{
	int x = 0, y = r,d;
	d = 3 - 2 * r;
	while (x <= y) 
	{
		_draw_circle_8(xc, yc, x, y, color);
		if (d < 0) 
		{
				d = d + 4 * x + 6;
		}
		else 
		{
				d = d + 4 * (x - y) + 10;
				y--;
		}
		x++;
	}
	OLED_Display();
}


/*****************************************************************************
 * @name       :void GUI_FillCircle(uint8_t xc, uint8_t yc, uint8_t color, uint8_t r)
 * @function   :在指定位置填充指定大小的圆圈
 * @parameters :xc:圆心的x坐标
                yc:圆心的y坐标
				r:半径
				color:圆的颜色值 1-白色 0-黑色
 * @retvalue   :无
******************************************************************************/  
void GUI_FillCircle(uint8_t xc, uint8_t yc, uint8_t color, uint8_t r)
{
	int x = 0, y = r, yi, d;
	d = 3 - 2 * r;
	while (x <= y) 
	{
			for (yi = x; yi <= y; yi++)
			{
				_draw_circle_8(xc, yc, x, yi, color);
			}
			if (d < 0) 
			{
				d = d + 4 * x + 6;
			} 
			else 
			{
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
	}
	OLED_Display();
}

/**********************************************************************************
 * @name       :void GUI_DrawTriangel(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color)
 * @function   :在指定的位置画一个三角形
 * @parameters :x0:三角形边的起始x坐标
                y0:三角形边的起始y坐标
				x1:三角形的顶点x坐标
				y1:三角形的顶点y坐标
				x2:三角形边的结束x坐标
				y2:三角形边的结束y坐标
				color:圆的颜色值 1-白色 0-黑色
 * @retvalue   :无
***********************************************************************************/ 
void GUI_DrawTriangel(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color)
{
	GUI_DrawLine(x0,y0,x1,y1,color);
	GUI_DrawLine(x1,y1,x2,y2,color);
	GUI_DrawLine(x2,y2,x0,y0,color);
}


/*****************************************************************************
 * @name       :static void _swap(uint8_t *a, uint8_t *b)
 * @function   :交换两个数字(内部呼叫)
 * @parameters :a:第一个数字的地址
                b:第二个数字的地址
 * @retvalue   :无
******************************************************************************/  
static void _swap(uint8_t *a, uint8_t *b)
{
	u16 tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

/*****************************************************************************
 * @name       :static void _draw_h_line(uint8_t x0,uint8_t x1,uint8_t y,uint8_t color)
 * @function   :在RAM中画一条水平线(内部调用)
 * @parameters :x0:水平线的起始x坐标
                x1:水平线的终点x坐标
				y:水平线的y坐标
				color:线的颜色值 1-白色 0-黑色
 * @retvalue   :无
******************************************************************************/
static void _draw_h_line(uint8_t x0,uint8_t x1,uint8_t y,uint8_t color)
{
	uint8_t i=0;
	for(i=x0;i<=x1;i++)
	{
		OLED_Set_Pixel(i, y, color);
	}
}


/*****************************************************************************
 * @name       :void GUI_FillTriangel(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color)
 * @function   :在指定位置填充三角形
 * @parameters :x0:三角形边的起始x坐标
                y0:三角形边的起始y坐标
                x1:三角形的顶点x坐标
                y1:三角形的顶点y坐标
                x2:三角形边的结束x坐标
                y2:三角形边的结束y坐标
                color:三角形的颜色值 1-白色 0-黑色
 * @retvalue   :无
******************************************************************************/ 
void GUI_FillTriangel(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color)
{
	uint8_t a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
 	}
 	if (y1 > y2) 
	{
    _swap(&y2,&y1); 
		_swap(&x2,&x1);
 	}
  if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
  }
	if(y0 == y2) 
	{ 
		a = b = x0;
		if(x1 < a)
    {
			a = x1;
    }
    else if(x1 > b)
    {
			b = x1;
    }
    if(x2 < a)
    {
			a = x2;
    }
		else if(x2 > b)
    {
			b = x2;
    }
		_draw_h_line(a,b,y0,color);
    return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	
	if(y1 == y2)
	{
		last = y1; 
	}
	else
	{
		last = y1-1; 
	}
	for(y=y0; y<=last; y++) 
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
    sb += dx02;
    if(a > b)
    {
			_swap(&a,&b);
		}
		_draw_h_line(a,b,y,color);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) 
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b)
		{
			_swap(&a,&b);
		}
		_draw_h_line(a,b,y,color);
	}
	OLED_Display();
}


/*****************************************************************************
 * @name       :void GUI_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t mode)
 * @function   :显示单个英文字符
 * @parameters :x:字符显示位置的起始x坐标
                y:字符显示位置的起始y坐标
				chr:显示字符的ASCII码(0~94)
				Char_Size:显示字符的大小(8,16)
				mode:0-白底黑字 1-黑底字白
 * @retvalue   :无
******************************************************************************/ 
void GUI_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t mode)
{      	
	unsigned char c=0,i=0,tmp,j=0;	
	c=chr-' ';//得到偏移后的值			
	if(x>WIDTH-1){x=0;y=y+2;}
	if(Char_Size ==16)
	{
		for(i=0;i<16;i++)
		{
			if(mode)
			{
				tmp = F8X16[c*16+i];
			}
			else
			{
				tmp = ~(F8X16[c*16+i]);
			}
			for(j=0;j<8;j++)
			{
				if(tmp&(0x80>>j))
				{
					OLED_Set_Pixel(x+j, y+i,1);
				}
				else
				{
					OLED_Set_Pixel(x+j, y+i,0);
				}
			}
		}
	}
	else if(Char_Size==8)
	{	
		for(i=0;i<8;i++)
		{
			if(mode)
				{
					tmp = F6x8[c][i];
				}
				else
				{
					tmp = ~(F6x8[c][i]);
				}
				for(j=0;j<8;j++)
				{
					if(tmp&(0x80>>j))
					{
						OLED_Set_Pixel(x+j, y+i,1);
					}
					else
					{
						OLED_Set_Pixel(x+j, y+i,0);
					}
			}
		}
	}
	else
	{
		return;
	}
	OLED_Display();
}


/*****************************************************************************
 * @name       :void GUI_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size,uint8_t mode)
 * @function   :显示英文字符串
 * @parameters :x:英语字符串的起始x坐标
                y:英语字符串的起始y坐标
				chr:英文字符串的起始地址
				Char_Size:显示字符的大小
				mode:0-白底黑字 1-黑底字白
 * @retvalue   :无
******************************************************************************/   	  
void GUI_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size,uint8_t mode)
{
	unsigned char j=0,csize;
	if(Char_Size == 16)
	{
		csize = Char_Size/2;
	}
	else if(Char_Size == 8)
	{
		csize = Char_Size/2+2;
	}
	else
	{
		return;
	}
	while (chr[j]!='\0')
	{		
		GUI_ShowChar(x,y,chr[j],Char_Size,mode);
		x+=csize;
		if(x>120)
		{
			x=0;
			y+=Char_Size;
		}
		j++;
	}
}


/*****************************************************************************
 * @name       :u32 mypow(uint8_t m,uint8_t n)
 * @function   :得到m的n次幂(内部调用)
 * @parameters :m: 乘数
                n:权
 * @retvalue   :m的n次幂
******************************************************************************/ 
static u32 mypow(uint8_t m,uint8_t n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}


/*****************************************************************************
 * @name       :void GUI_ShowNum(uint8_t x,uint8_t y,u32 num,uint8_t len,uint8_t Size,uint8_t mode)
 * @function   :显示数 
 * @parameters :x:这个数的起始x坐标
                y:这个数的起始y坐标
				num:数字(0 ~ 4294967295)
				len:显示长度
				Size:显示数的大小
				mode:0-白底黑字 1-黑底字白
 * @retvalue   :无
******************************************************************************/  			 
void GUI_ShowNum(uint8_t x,uint8_t y,u32 num,uint8_t len,uint8_t Size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0,csize;
	if(Size == 16)
	{
		csize = Size/2;
	}
	else if(Size == 8)
	{
		csize = Size/2+2;
	} 	
	else
	{
		return;
	}
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				GUI_ShowChar(x+csize*t,y,' ',Size,mode);
				continue;
			}else enshow=1; 
		 	 
		}
	 	GUI_ShowChar(x+csize*t,y,temp+'0',Size,mode); 
	}
}  


/*****************************************************************************
 * @name       :void GUI_ShowFont16(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode)
 * @function   :显示单个16x16的汉字
 * @parameters :x:汉字的起始x坐标
                y:汉字的起始y坐标
				s:中文字符的起始地址
				mode:0-白底黑字 1-黑底字白
 * @retvalue   :无
******************************************************************************/ 
void GUI_ShowFont16(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode)
{
	uint8_t i,j,k,tmp;
	u16 num;
	num = sizeof(cfont16)/sizeof(typFNT_GB16);
	for(i=0;i<num;i++)
	{
		if((cfont16[i].Index[0]==*s)&&(cfont16[i].Index[1]==*(s+1)))
		{
			for(j=0;j<32;j++)
			{
				if(mode)
				{
					tmp = cfont16[i].Msk[j];
				}
				else
				{
					tmp = ~(cfont16[i].Msk[j]);
				}
				for(k=0;k<8;k++)
				{
					if(tmp&(0x80>>k))
					{
						OLED_Set_Pixel(x+(j%2)*8+k, y+j/2,1);
					}
					else
					{
						OLED_Set_Pixel(x+(j%2)*8+k, y+j/2,0);
					}
				}
			}	
			break;
		}	
	}
	OLED_Display();
}

/*****************************************************************************
 * @name       :void GUI_ShowFont24(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode)
 * @function   :显示一个24x24的中文字符
 * @parameters :x:汉字的起始x坐标
                y:汉字的起始y坐标
				s:中文字符的起始地址
				mode:0-白底黑字 1-黑底字白
 * @retvalue   :无
******************************************************************************/ 
void GUI_ShowFont24(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode)
{
	uint8_t i,j,k,tmp;
	u16 num;
	num = sizeof(cfont24)/sizeof(typFNT_GB24);
	for(i=0;i<num;i++)
	{
		if((cfont24[i].Index[0]==*s)&&(cfont24[i].Index[1]==*(s+1)))
		{
			for(j=0;j<72;j++)
			{
				if(mode)
				{
					tmp = cfont24[i].Msk[j];
				}
				else
				{
					tmp = ~(cfont24[i].Msk[j]);
				}
				for(k=0;k<8;k++)
				{
					if(tmp&(0x80>>k))
					{
						OLED_Set_Pixel(x+(j%3)*8+k, y+j/3,1);
					}
					else
					{
						OLED_Set_Pixel(x+(j%3)*8+k, y+j/3,0);
					}
				}
			}	
			break;
		}	
	}
	OLED_Display();
}


/*****************************************************************************
 * @name       :void GUI_ShowFont32(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode)
 * @function   :显示单个32x32的中文字符
 * @parameters :x:汉字的起始x坐标
                y:汉字的起始y坐标
				s:中文字符的起始地址
				mode:0-白底黑字 1-黑底字白
 * @retvalue   :无
******************************************************************************/ 
void GUI_ShowFont32(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode)
{
	uint8_t i,j,k,tmp;
	u16 num;
	num = sizeof(cfont32)/sizeof(typFNT_GB32);
	for(i=0;i<num;i++)
	{
		if((cfont32[i].Index[0]==*s)&&(cfont32[i].Index[1]==*(s+1)))
		{
			for(j=0;j<128;j++)
			{
				if(mode)
				{
					tmp = cfont32[i].Msk[j];
				}
				else
				{
					tmp = ~(cfont32[i].Msk[j]);
				}
				for(k=0;k<8;k++)
				{
					if(tmp&(0x80>>k))
					{
						OLED_Set_Pixel(x+(j%4)*8+k, y+j/4,1);
					}
					else
					{
						OLED_Set_Pixel(x+(j%4)*8+k, y+j/4,0);
					}
				}
			}	
			break;
		}	
	}
	OLED_Display();
}

/*****************************************************************************
 * @name       :void GUI_ShowCHinese(uint8_t x,uint8_t y,uint8_t hsize,uint8_t *str,uint8_t mode)
 * @function   :显示汉字字符串
 * @parameters :x:汉字的起始x坐标
                y:汉字的起始y坐标
				size:汉字大小
				str:汉字字符串的起始地址
				mode:0-白底黑字 1-黑底字白
 * @retvalue   :无
******************************************************************************/	   		   
void GUI_ShowCHinese(uint8_t x,uint8_t y,uint8_t hsize,uint8_t *str,uint8_t mode)
{ 
	while(*str!='\0')
	{
		if(hsize == 16)
		{
			GUI_ShowFont16(x,y,str,mode);
		}
		else if(hsize == 24)
		{
			GUI_ShowFont24(x,y,str,mode);
		}
		else if(hsize == 32)
		{
			GUI_ShowFont32(x,y,str,mode);
		}
		else
		{
			return;
		}
		x+=hsize;
		if(x>WIDTH-hsize)
		{
			x=0;
			y+=hsize;
		}
		str+=2;
	}			
}

/*****************************************************************************
 * @name       :void GUI_DrawBMP(uint8_t x,uint8_t y,uint8_t width, uint8_t height, uint8_t BMP[], uint8_t mode)
 * @function   :显示BMP单色图片
 * @parameters :x:BMP单色图像的起始x坐标
                y:BMP单色图像的起始y坐标
				width:BMP单色图片的宽度
				height:BMP单色图片的高度
				BMP:BMP单色图像阵列的起始地址
				mode:0-白底黑字 1-黑底字白
 * @retvalue   :无								
******************************************************************************/ 
void GUI_DrawBMP(uint8_t x,uint8_t y,uint8_t width, uint8_t height, uint8_t BMP[], uint8_t mode)
{ 	
	uint8_t i,j,k;
	uint8_t tmp;
	for(i=0;i<height;i++)
	{
		for(j=0;j<(width+7)/8;j++)
		{
		    if(mode)
			{
				tmp = BMP[i*((width+7)/8)+j];
			}
			else
			{
				tmp = ~BMP[i*((width+7)/8)+j];
			}
			for(k=0;k<8;k++)
			{
				if(tmp&(0x80>>k))
				{
					OLED_Set_Pixel(x+j*8+k, y+i,1);
				}
				else
				{
					OLED_Set_Pixel(x+j*8+k, y+i,0);
				}
			}
		}
	} 
	OLED_Display();
}

