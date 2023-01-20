/********************************************************
实验名称：	DMA传输实验（Peripheral->SRAM）

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  无

实验现象：	电脑向串口发送数据，数据会返回到电脑

更新时间：  2022-08-7
********************************************************/
#include "dma.h"
#include "led.h"
#include "usart1.h"

int main(void)
{
	Delay_Init();//延时函数初始化
	LED_Hardware_Init();    //LED 和 Beep 初始化
	USART1_Hardware_Init(115200);//DMA_USART 初始化
	DMA_Hardware_Init();             //DMA初始化
    USART1_Send_String("DMA(P->M)模式，电脑向串口发送数据，数据会返回到电脑\r\n");

	while(1)
	{	
		LED_R(0);
	}

}

