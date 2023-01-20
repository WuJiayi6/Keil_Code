/********************************************************
实验名称：	温湿度实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  在U16接入DS18B20模块

实验现象：	LCD液晶屏显示对应的温度值

更新时间：  2022-07-27
********************************************************/
#include "led.h"
#include "bsp_ili9341_lcd.h"
#include "DS18B20.h"


int main(void)
{	
	float Temperature;
	char buff[100];
	Delay_Init();//延时函数初始化
	ILI9341_Init();//LCD初始化
	DS18B20_Init();//DS18B20初始化
	while(1)
	{	
		Temperature = DS18B20_GetTemp_SkipROM();
		sprintf((char *)buff,"Temperature :   %.3f",Temperature);
		ILI9341_DispStringLine_EN(LINE(1),buff);

		Delay_ms(500);
	}
}


