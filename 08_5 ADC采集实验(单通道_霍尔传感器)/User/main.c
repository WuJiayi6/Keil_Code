/********************************************************
实验名称：	ADC实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  基于自制野火JP4拓展板

实验现象：	LCD液晶屏显示ADC值

更新时间：  2022-07-21
********************************************************/
#include "adc1.h"


int main(void)
{	

	Delay_Init();//延时函数初始化
	LED_Hardware_Init();
	ADC1_Hardware_Init();
	ILI9341_Init();//LCD初始化
	while(1)
	{		
		Hall_Test();
	}
}
