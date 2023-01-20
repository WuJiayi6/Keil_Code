/********************************************************
实验名称：	定时器中断实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  

实验现象：	使用TIM3，通过串口显示24小时时钟

更新时间：2022-07-12
********************************************************/

#include "TIM3.h"

int main(void)
{
    Delay_Init();
	LED_Hardware_Init();//LED 和 Beep 初始化
	USART1_Hardware_Init(115200);//串口初始化
    USART1_Send_String("\n\n24小时时钟\n\n\n");
    Delay_ms(1000);
	TIM3_Hardware_Init(10000-1,7200-1);//定时器3初始化,1s
	while(1)
	{
		
	}

}


