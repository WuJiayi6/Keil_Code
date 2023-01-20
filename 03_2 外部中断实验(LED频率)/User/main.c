/********************************************************
实验名称：	外部中断实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  无

实验现象：	外部中断，按键控制LED闪烁频率

更新时间：2022-07-12
********************************************************/
#include "bsp_EXTI.h"

int main(void)
{
	LED_Hardware_Init();//LED 和 Beep 初始化
	Key_Hardware_Init();//按键初始化
	EXTI_Hardware_Init();//外部中断初始化
	Delay_Init();       //延时初始化

	while(1)
	{
		LED_Frequency_Display();
	}
}

