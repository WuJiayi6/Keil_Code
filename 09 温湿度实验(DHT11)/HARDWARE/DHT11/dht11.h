#ifndef __DHT11_H
#define __DHT11_H 

#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "bsp_ili9341_lcd.h"
#include <stdio.h>

//IO��������
#define DHT11_IO_IN()    DQ_IN()
#define DHT11_IO_OUT()   DQ_OUT()

////IO��������			  					    			   
#define	DHT11_DQ_OUT(X) GPIO_WriteBit(GPIOE,GPIO_Pin_6,(BitAction)X)//���ݶ˿�	PE6
#define	DHT11_DQ_IN()  	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)//���ݶ˿�	PE6

uint8_t DHT11_Init(void);					//��ʼ��DHT11
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi);	//��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);				//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);				//����һ��λ
uint8_t DHT11_Check(void);					//����Ƿ����DHT11
void DHT11_Rst(void);						//��λDHT11   
void DQ_OUT(void);
void DQ_IN(void);
void DHT11_Test(void);
void DHT11_WHILE(void);
	

#endif
 
