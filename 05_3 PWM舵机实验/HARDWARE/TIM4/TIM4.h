#ifndef _TIM4_H
#define _TIM4_H

#include "stm32f10x.h"
#include "key.h" 
extern float Angle;
void TIM4_PWM_Hardware_Init(uint16_t arr ,uint16_t psc);
void TIM4_IRQHandler(void);
void TIM4_PWM_SetCompare3(float Compare);
void PWM_Servo_Test(float Angle);

#endif
