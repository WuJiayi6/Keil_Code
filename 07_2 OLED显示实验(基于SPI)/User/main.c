/********************************************************
实验名称：	LCD显示实验(基于SPI)

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  接自制野火OLED拓展板

实验现象：	LCD屏显示 "Happy Mid-Autumn Festival"
			OLED屏显示 "中秋快乐"

更新时间：  2022-07-19
********************************************************/
#include "TIM3.h"
#include "gui.h"
#include "bmp.h"

int main(void)
{	

	Delay_Init();//延时函数初始化
	ILI9341_Init();
	TIM3_Hardware_Init(10000,7200);//1s
	LED_Hardware_Init();//LED初始化
    OLED_Init();		//初始化OLED  
    OLED_Clear(0);      //清屏（全黑）
	/***********************************************************************************/
	/**///①刚上电，产品芯片内部就绪需要时间，延时100~500ms,建议500ms
	/***********************************************************************************/
	Delay_ms(500);	
	GUI_ShowCHinese(0,10,24,"中秋快乐",1);
    OLED_WR_Byte(0x2E,OLED_CMD); //关闭滚动
    OLED_WR_Byte(0x27,OLED_CMD); //水平向左或者右滚动 26/27
    OLED_WR_Byte(0x00,OLED_CMD); //虚拟字节
    OLED_WR_Byte(0x00,OLED_CMD); //起始页 0
    OLED_WR_Byte(0x07,OLED_CMD); //滚动时间间隔
    OLED_WR_Byte(0x07,OLED_CMD); //终止页 2
    OLED_WR_Byte(0x00,OLED_CMD); //虚拟字节
    OLED_WR_Byte(0xFF,OLED_CMD); //虚拟字节
	OLED_WR_Byte(0x2F,OLED_CMD); //开启滚动
	LCD_SetFont(&Font8x16);
	ILI9341_DispStringLine_EN(LINE(5),"Happy Mid-Autumn Festival");//LCD显示
	while(1)
	{	
		LCD_SetFont(&Font16x24);
		TIM3_Counter();
	}

}


