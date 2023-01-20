/********************************************************
实验名称：	LCD显示实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  LCD 驱动芯片 LCDID_ST7789V   0x8552

实验现象：	LCD液晶屏显示 "Hello World" 和 24小时时钟

更新时间：  2022-07-19
********************************************************/
#include "delay.h"
#include "TIM3.h"
#include "bsp_ili9341_lcd.h"

int main(void)
{	

	Delay_Init();//延时函数初始化
	TIM3_Hardware_Init(10000-1,7200-1);//定时器1s
	ILI9341_Init();//LCD初始化
    LCD_SetFont(&Font8x16);//显示字体大小
	ILI9341_DispStringLine_EN_CH(LINE(1),"你好");
    LCD_SetFont(&Font16x24);//显示字体大小
    ILI9341_DispStringLine_EN(LINE(2),"Hello World!");
	while(1)
	{	
		TIM3_24_Clock();
	}

}


