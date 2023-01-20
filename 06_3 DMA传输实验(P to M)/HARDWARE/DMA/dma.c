#include "dma.h"


/****************************************************************
��  �ܣ�USART1_DMA����
��  ������
����ֵ����
*****************************************************************/									
void DMA_Hardware_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);// ����DMAʱ��
	
	//Դ���ݵ�ַ,����Ĵ�����ַ,USART1����ַ+ƫ�Ƶ�ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (USART1_BASE+0x04);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart1_buffer;// Ŀ���ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// ���򣺴����赽�ڴ�
	DMA_InitStructure.DMA_BufferSize = USART1_RX_SIZE;// �����С	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// �����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// �ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// �������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// �ڴ����ݵ�λ	 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // DMAģʽ��ѭ��ģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;// ���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// ��ֹ�ڴ浽�ڴ�Ĵ���

	DMA_Init(DMA1_Channel5, &DMA_InitStructure);// ����Rx��Ӧ��DMA����ͨ��
	DMA_Cmd(DMA1_Channel5,ENABLE);// ʹ��DMA
    
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);/* USART1 �� DMA����RX���� */
}									
	





