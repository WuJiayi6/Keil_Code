#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"
#include  <stdio.h>

static uint16_t adcx = 0;		//���ڴ�����ݵı���
static char show[70]= {0};								//���ڴ�Ŵ�ӡ���ݵ�����

void ADC_Hardware_Init(void);				//ADC��ʼ�����ú���
uint16_t ADC_Collect(uint8_t ch);			//ADC�ɼ�����

#endif

