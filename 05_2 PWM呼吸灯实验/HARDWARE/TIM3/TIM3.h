#ifndef _TIM3_H
#define _TIM3_H

#include "stm32f10x.h"
#include "led.h" 

void TIM3_PWM_Hardware_Init(uint16_t arr ,uint16_t psc);
void PWM_LED_Breath_Test(void);

#endif
