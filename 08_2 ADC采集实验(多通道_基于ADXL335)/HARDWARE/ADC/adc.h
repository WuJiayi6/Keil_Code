#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"
#include  <stdio.h>

static uint16_t adcx = 0;		//���ڴ�����ݵı���
static uint16_t adcy = 0;		//���ڴ�����ݵı���
static uint16_t adcz = 0;		//���ڴ�����ݵı���

static char show1[70]= {0};		//���ڴ�Ŵ�ӡ���ݵ�����
static char show2[70]= {0};		//���ڴ�Ŵ�ӡ���ݵ�����
static char show3[70]= {0};		//���ڴ�Ŵ�ӡ���ݵ�����

void ADC_Hardware_Init(void);				//ADC��ʼ�����ú���
uint16_t ADC_Collect(uint8_t ADC_Channel);			//ADC�ɼ�����

#endif

