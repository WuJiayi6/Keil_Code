#include "OLED.h"

/*******************************************************************
 * @name       :void OLED_WR_Byte(unsigned dat,unsigned cmd)
 * @function   :将一个字节的内容写入OLED屏幕
 * @parameters :dat:要写的内容
                cmd:0-写命令 1-写数据
 * @retvalue   :无
********************************************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		OLED_DC_Set();
	}
	else
	{
		OLED_DC_Clr();
	}
	OLED_CS_Clr();
	SPI_WriteByte(dat);
	OLED_CS_Set();
}


/*******************************************************************
 * @name       :void OLED_Set_Pos(unsigned char x, unsigned char y) 
 * @function   :在OLED屏幕上设置坐标
 * @parameters :x: X坐标
                y: Y坐标
 * @retvalue   :无
********************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
 	OLED_WR_Byte(YLevel+y/PAGE_SIZE,OLED_CMD);
	OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}  


/*******************************************************************
 * @name       :void OLED_Display_On(void) 
 * @function   :打开OLED显示屏
 * @parameters :无
 * @retvalue   :无
********************************************************************/ 	  
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}


/*******************************************************************
 * @name       :void OLED_Display_Off(void)
 * @function   :关闭OLED显示屏
 * @parameters :无
 * @retvalue   :无
********************************************************************/    
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}


/*******************************************************************
 * @name       :void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color)
 * @function   :设置像素值为RAM
 * @parameters :x:像素的x坐标
                y:像素的y坐标
				color:点的颜色值 1-白色 0-黑色
 * @retvalue   :无
********************************************************************/ 
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color)
{
	if(color)
	{
		OLED_buffer[(y/PAGE_SIZE)*WIDTH+x]|= (1<<(y%PAGE_SIZE))&0xff;
	}
	else
	{
		OLED_buffer[(y/PAGE_SIZE)*WIDTH+x]&= ~((1<<(y%PAGE_SIZE))&0xff);
	}
}		   			 


/*******************************************************************
 * @name       :void OLED_Display(void)
 * @function   :OLED屏幕显示
 * @parameters :无
 * @retvalue   :无
********************************************************************/  
void OLED_Display(void)
{
	u8 i,n;		    
	for(i=0;i<PAGE_SIZE;i++)  
	{  
		OLED_WR_Byte (YLevel+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (XLevelL,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (XLevelH,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<WIDTH;n++)
		{
			OLED_WR_Byte(OLED_buffer[i*WIDTH+n],OLED_DATA); 
		}
	}   //更新显示
}


/*******************************************************************
 * @name       :void OLED_Clear(unsigned dat)  
 * @function   :清除OLED屏幕
 * @parameters :dat:0-显示全黑
                    1-显示全白
 * @retvalue   :无
********************************************************************/ 
void OLED_Clear(unsigned dat)  
{  
	if(dat)
	{
		memset(OLED_buffer,0xff,sizeof(OLED_buffer));
	}
	else
	{
		memset(OLED_buffer,0,sizeof(OLED_buffer));
	}
	OLED_Display();
}


/*******************************************************************
 * @name       :void OLED_Reset(void) 
 * @function   :重置OLED屏幕
 * @parameters :dat:0-显示全黑
                    1-显示全白
 * @retvalue   :无
********************************************************************/ 
void OLED_Reset(void)
{
	OLED_RST_Set();
	Delay_ms(100);
	OLED_RST_Clr();
	Delay_ms(100);
	OLED_RST_Set(); 
}	


/*******************************************************************
 * @name       :void OLED_Hardware_Init(void)
 * @function   :复位OLED屏
 * @parameters :无
 * @retvalue   :无
********************************************************************/ 
void OLED_Hardware_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);//使能BC端口时钟
	
	/************    CS   ******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//GPIOB12 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB12

	/************    RES   ******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOC6
	
	/************    DC  控制写命令还是写数据  ******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOC7
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);//拉高
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);//拉高
}

/*******************************************************************
 * @name       :void OLED_Init(void)
 * @function   :初始化OLED SSD1306控制IC
 * @parameters :无
 * @retvalue   :无
********************************************************************/ 				    
void OLED_Init(void)
{
	SPI2_Hardware_Init();  //初始化SPI2
 	OLED_Hardware_Init(); //初始化GPIO
 	Delay_ms(200);
	OLED_Reset();     //复位OLED
	
/**************初始化SSD1306*****************/	
	OLED_WR_Byte(0xAE,OLED_CMD); /*关闭显示*/
	OLED_WR_Byte(0x00,OLED_CMD); /*设置低列地址*/
	OLED_WR_Byte(0x10,OLED_CMD); /*设置高列地址*/
	OLED_WR_Byte(0x40,OLED_CMD); /*设置显示起始行*/ 
	OLED_WR_Byte(0xB0,OLED_CMD); /*设置页地址*/
	OLED_WR_Byte(0x81,OLED_CMD); /*联合控制*/ 
	OLED_WR_Byte(0xFF,OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1,OLED_CMD); /*设置部分重新映射*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x3F,OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com 扫描方向*/
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0XF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x30,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD); /*set charge pump disable*/ 
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD); /* 开显示*/
}  

