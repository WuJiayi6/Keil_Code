/********************************************************
实验名称：	DMA传输实验（FLASH->SRAM）

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  无

实验现象：	复位亮黄灯，DMA传输完成亮绿灯

更新时间：2022-07-13
********************************************************/
#include "dma.h"
#include "led.h"

int main(void)
{
	
	LED_Hardware_Init();    //LED 和 Beep 初始化
	DMA_Hardware_Init();//DMA初始化
	Delay_Init();//延时函数初始化
	LED_Y(0);
	Delay_ms(1000);
	LED_W(1);//关闭所有的灯
	while(1)
	{	
		TransferStatus = Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);
		/* 等待DMA传输完成 */
		while(DMA_GetFlagStatus(DMA1_FLAG_TC6)==RESET) {}
		if(TransferStatus)
			LED_G(0);
		else
			LED_R(0);
	}

}


