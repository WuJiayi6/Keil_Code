#ifndef _TIM4_H
#define _TIM4_H

#include "stm32f10x.h"
#include "usart1.h"
#include "led.h"
#include "delay.h"

extern uint32_t status;						// 计数值
extern uint32_t real_time;					// 读回值
extern float dis_temp;						// 距离计算值

void TIM4_Hardware_Init(uint16_t arr,uint16_t psc);
void Tran_EXTI_Init(void);
void Ultrasonic_Wave(void);
#endif
