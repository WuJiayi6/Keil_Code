#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#include "stm32f10x.h"
#include "led.h"

void EXTI_Hardware_Init(void);
void EXTI0_IRQHandler(void);//�ⲿ�ж�0������
void EXTI15_10_IRQHandler(void);//�ⲿ�ж�13������

#endif
/*__bsp_EXIT_H*/

