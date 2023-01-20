#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"
#include  <stdio.h>

static uint16_t adcx = 0;		//用于存放数据的变量
static uint16_t adcy = 0;		//用于存放数据的变量
static uint16_t adcz = 0;		//用于存放数据的变量

static char show1[70]= {0};		//用于存放打印数据的数组
static char show2[70]= {0};		//用于存放打印数据的数组
static char show3[70]= {0};		//用于存放打印数据的数组

void ADC_Hardware_Init(void);				//ADC初始化配置函数
uint16_t ADC_Collect(uint8_t ADC_Channel);			//ADC采集函数

#endif

