#ifndef __USART1_H_
#define __USART1_H_

#include "stm32f10x.h"


#define USART1_RX_SIZE     5000     // 要发送的数据大小

extern uint8_t usart1_buffer[USART1_RX_SIZE];//储存串口一接收数据的数组
extern uint16_t usart1_length;//储存串口一接收数据数组的长度					

void USART1_Hardware_Init(uint32_t baudrate);//串口1的初始化函数
void USART1_Send_Byte(uint8_t src);	//串口1发送单字节数据
void USART1_Send_Length_String(uint8_t *src,uint16_t length);//串口1发送一定长度的字符串数据
void USART1_Send_String(char *src);//串口1发送字符串数据
void USART1_Clear_Buffer(void);//串口1清除储存数据

#endif
