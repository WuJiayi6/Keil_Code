#ifndef _TIM4_H
#define _TIM4_H

#include "stm32f10x.h"
#include "usart1.h"
#include "led.h"
#include "delay.h"

extern uint32_t status;						// ����ֵ
extern uint32_t real_time;					// ����ֵ
extern float dis_temp;						// �������ֵ

void TIM4_Hardware_Init(uint16_t arr,uint16_t psc);
void Tran_EXTI_Init(void);
void Ultrasonic_Wave(void);
#endif
