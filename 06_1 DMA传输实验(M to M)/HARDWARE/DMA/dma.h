#ifndef __DMA_H_
#define __DMA_H_

#include "stm32f10x.h"


#define BUFFER_SIZE     32     // 要发送的数据大小

void DMA_Hardware_Init(void);
uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBuffer1, uint16_t BufferLength);

extern uint8_t TransferStatus;/* 定义存放比较结果变量 */
extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];/* 定义DMA传输目标存储器,存储在内部的SRAM中 */
#endif

