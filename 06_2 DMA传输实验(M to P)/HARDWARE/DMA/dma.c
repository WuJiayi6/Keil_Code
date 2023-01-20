#include "dma.h"


//USART1_DMA配置									
void DMA_Hardware_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);// 开启DMA时钟
	
	//源数据地址,外设寄存器地址,USART1基地址+偏移地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (USART1_BASE+0x04);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart1_buffer;// 目标地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;// 方向：从内存到外设
	DMA_InitStructure.DMA_BufferSize = USART1_RX_SIZE;// 传输大小	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 外设地址不增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// 外设数据单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 内存数据单位	 
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;// DMA模式，一次
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // DMA模式，循环模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;// 优先级：高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// 禁止内存到内存的传输

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);//// 串口对应的DMA请求通道  DMA1_Channel4
	DMA_Cmd(DMA1_Channel4,ENABLE);// 使能DMA
}									
	





