#ifndef __DMA_H_
#define __DMA_H_

#include "stm32f10x.h"


#define BUFFER_SIZE     32     // Ҫ���͵����ݴ�С

void DMA_Hardware_Init(void);
uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBuffer1, uint16_t BufferLength);

extern uint8_t TransferStatus;/* �����űȽϽ������ */
extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];/* ����DMA����Ŀ��洢��,�洢���ڲ���SRAM�� */
#endif

