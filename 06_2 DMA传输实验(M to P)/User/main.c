/********************************************************
实验名称：	DMA传输实验（FLASH->Peripheral）

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  无

实验现象：	SRAM(M)通过串口将SendBuff[]数组发出，同时黄灯一直闪烁，
			数据通过DMA(M->P)传输不占用MCU

更新时间：2022-07-14
********************************************************/
#include "dma.h"
#include "led.h"
#include "usart1.h"

int main(void)
{
	uint16_t i = 0;//用于写入SendBuff[]数组
	
	LED_Hardware_Init();    //LED 和 Beep 初始化
	DMA_USART1_Hardware_Init(115200);//DMA_USART 初始化
	DMA_Hardware_Init();             //DMA初始化
	Delay_Init();//延时函数初始化

	for(i=0;i<USART1_RX_SIZE;i++)
		usart1_buffer[i] = '1';
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	while(1)
	{	
		
		LED_Y(0);
		Delay_ms(500);
		LED_W(1);//关闭所有的灯
		Delay_ms(500);

	}

}


