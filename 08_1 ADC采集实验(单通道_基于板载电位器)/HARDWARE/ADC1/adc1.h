#ifndef __ADC1_H_
#define __ADC1_H_

#include "stm32f10x.h"
#include  <stdio.h>

static uint16_t adcx = 0;		//���ڴ�����ݵı���
static char show[70]= {0};		//���ڴ�Ŵ�ӡ���ݵ�����

void ADC1_Hardware_Init(void);				//ADC��ʼ�����ú���
uint16_t ADC1_Collect(void);			//ADC�ɼ�����

#endif

