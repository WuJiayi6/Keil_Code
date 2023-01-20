#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "stm32f10x.h"
#include "delay.h"


static float Temperature;
static char buff[100];
	
	
uint8_t DS18B20_Init(void);
void DS18B20_Hardware_Init(void);
void DS18B20_IN_IPU(void);
void DS18B20_OUT_PP(void);
void DS18B20_Reset(void);
uint8_t DS18B20_Presence(void);
uint8_t DS18B20_Read_Bit(void);
uint8_t DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(uint8_t Data);
void DS18B20_Skip_ROM(void);
void DS18B20_Match_ROM(void);
float DS18B20_GetTemp_SkipROM(void);
void DS18B20_ReadId(uint8_t * DS18B20_ID);
float DS18B20_GetTemp_MatchROM ( uint8_t * DS18B20_ID);

#endif

