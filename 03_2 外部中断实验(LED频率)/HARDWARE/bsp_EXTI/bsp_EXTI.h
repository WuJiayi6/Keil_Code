#ifndef __BSP_EXTI_H__
#define __BSP_EXTI_H__

  
#include "led.h"
#include "key.h"
#include "stm32f10x.h"

static uint16_t time;
static uint8_t flag=0;

void EXTI_Hardware_Init(void);
void LED_Frequency_Display(void);

#endif
/*__bsp_EXIT_H*/

