/********************************************************
实验名称：	串口通信实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  将USB-B线接USB转串口

实验现象：	通过串口发送指令控制LED灯
			发送"left"-->RGB灯以500ms循环
			发送"spark"-->R灯以500ms闪烁

更新时间：2022-07-12
********************************************************/

#include "stm32f10x.h"  
#include "usart1.h"


int main(void)
{
	LED_Hardware_Init();//LED 和 Beep 初始化
	USART1_Hardware_Init(115200);//串口初始化
	Delay_Init();       //延时初始化
	USART1_Send_String("please send \"left\" or \"spark\"\n ");
	while(1)
	{
		USART_Send_Command();
	}
}




