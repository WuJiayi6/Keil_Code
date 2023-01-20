#ifndef __USART1_H_
#define __USART1_H_

#include "stm32f10x.h"

//定义串口接受最大存储的数据为50个字节,可根据实际需要修改定义
#define USART1_RX_SIZE 50					

extern char usart1_buffer[];//储存串口一接收数据的数组
extern uint16_t usart1_length;//储存串口一接收数据数组的长度					

void USART1_Hardware_Init(uint32_t baudrate);//串口1的初始化函数
void USART1_Send_Byte(uint8_t src);	//串口1发送单字节数据
void USART1_Send_Length_String(uint8_t *src,uint16_t length);//串口1发送一定长度的字符串数据
void USART1_Send_String(char *src);//串口1发送字符串数据
void USART1_Clear_Buffer(void);//串口1清除储存数据

#endif
