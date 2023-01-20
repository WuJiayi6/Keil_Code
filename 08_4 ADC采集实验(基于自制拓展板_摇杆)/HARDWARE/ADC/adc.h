#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"
#include  <stdio.h>
#include "led.h"
#include "bsp_ili9341_lcd.h"

#define    CHANEL_NUM			3      // 转换通道个数

static char show0[40]= {0};		//用于存放打印数据的数组
static char show1[40]= {0};		//用于存放打印数据的数组
static char show2[40]= {0};		//用于存放打印数据的数组


extern volatile uint16_t ADC_Value[CHANEL_NUM];

void ADC1_GPIO_Init(void);                  //ADC1_IO初始化
void ADC1_Hardware_Init(void);				//ADC1初始化配置函数
void ADC1_Init(void);                       //ADC1初始化

void Init_All_Periph(void);                //初始化所有外设
void LCD_Show(void);
#endif

