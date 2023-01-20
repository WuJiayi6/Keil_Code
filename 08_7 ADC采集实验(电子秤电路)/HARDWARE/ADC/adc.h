#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"
#include  <stdio.h>
#include "bsp_ili9341_lcd.h"
#include "delay.h"

static uint16_t adcx = 0;		//采样值
static char show[70]= {0};								//用于存放打印数据的数组

void ADC_Hardware_Init(void);				//ADC初始化配置函数
uint16_t ADC_Collect(void);			//ADC采集函数
void Display_result(void);          //显示结果
#endif

