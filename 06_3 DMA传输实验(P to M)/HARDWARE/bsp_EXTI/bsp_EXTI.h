#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#include "stm32f10x.h"
#include "led.h"

void EXTI_Hardware_Init(void);
void EXTI0_IRQHandler(void);//外部中断0服务函数
void EXTI15_10_IRQHandler(void);//外部中断13服务函数

#endif
/*__bsp_EXIT_H*/

