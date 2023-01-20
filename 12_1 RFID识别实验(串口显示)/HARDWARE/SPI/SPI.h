#ifndef __SPI_H__
#define __SPI_H__

#include "sys.h"
#include "RC522.h"
#include "stm32f10x.h"

#define          RC522_SCK_0()             GPIO_ResetBits( GPIOA, GPIO_Pin_5 )
#define          RC522_SCK_1()             GPIO_SetBits ( GPIOA, GPIO_Pin_5 )
#define          RC522_MOSI_0()            GPIO_ResetBits( GPIOA, GPIO_Pin_7 )
#define          RC522_MOSI_1()            GPIO_SetBits ( GPIOA, GPIO_Pin_7 )
#define          RC522_MISO_GET()          GPIO_ReadInputDataBit ( GPIOA, GPIO_Pin_6 )

void SPI1_Hardware_Init(void);//SPI1初始化
void SPI_RC522_SendByte(uint8_t byte);//RC522通过SPI发送
uint8_t SPI_RC522_ReadByte(void);//RC522通过SPI读取 
    
#endif

