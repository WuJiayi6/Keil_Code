#ifndef __DHT11_H
#define __DHT11_H 

#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "bsp_ili9341_lcd.h"
#include <stdio.h>

//IO方向设置
#define DHT11_IO_IN()    DQ_IN()
#define DHT11_IO_OUT()   DQ_OUT()

////IO操作函数			  					    			   
#define	DHT11_DQ_OUT(X) GPIO_WriteBit(GPIOE,GPIO_Pin_6,(BitAction)X)//数据端口	PE6
#define	DHT11_DQ_IN()  	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)//数据端口	PE6

uint8_t DHT11_Init(void);					//初始化DHT11
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);	//读取温湿度
uint8_t DHT11_Read_Byte(void);				//读出一个字节
uint8_t DHT11_Read_Bit(void);				//读出一个位
uint8_t DHT11_Check(void);					//检测是否存在DHT11
void DHT11_Rst(void);						//复位DHT11   
void DQ_OUT(void);
void DQ_IN(void);
void DHT11_Test(void);
void DHT11_WHILE(void);
	

#endif
 
