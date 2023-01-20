#ifndef _TIM3_H__
#define _TIM3_H__

#include "stm32f10x.h"
#include "usart1.h"
#include "led.h"
#include <stdio.h>
#include "bsp_ili9341_lcd.h"
#include "DS18B20.h"

void TIM3_Hardware_Init(uint16_t arr,uint16_t psc);


#endif
