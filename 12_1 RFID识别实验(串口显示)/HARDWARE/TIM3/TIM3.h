#ifndef _TIM3_H
#define _TIM3_H

#include "stm32f10x.h"
#include "bsp_ili9341_lcd.h"
#include <stdio.h>

static uint8_t h=23,m=59,s=55,flag_TIM3 = 0;//Ԥ��ʱ����Ͷ�ʱ����־λ
static char Time_LCD[30];//���ڴ�ӡʱ��

void TIM3_Hardware_Init(uint16_t arr,uint16_t psc);
void TIM3_24_Clock(void);

#endif
