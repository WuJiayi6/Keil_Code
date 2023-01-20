#ifndef __USART1_H_
#define __USART1_H_

#include "stm32f10x.h"


#define USART1_RX_SIZE     5000     // Ҫ���͵����ݴ�С

extern uint8_t usart1_buffer[USART1_RX_SIZE];//���洮��һ�������ݵ�����
extern uint16_t usart1_length;//���洮��һ������������ĳ���					

void USART1_Hardware_Init(uint32_t baudrate);//����1�ĳ�ʼ������
void USART1_Send_Byte(uint8_t src);	//����1���͵��ֽ�����
void USART1_Send_Length_String(uint8_t *src,uint16_t length);//����1����һ�����ȵ��ַ�������
void USART1_Send_String(char *src);//����1�����ַ�������
void USART1_Clear_Buffer(void);//����1�����������

#endif
