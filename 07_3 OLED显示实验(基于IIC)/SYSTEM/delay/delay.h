#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void Delay_Init(void);
void Delay_us(uint16_t ctr);
void Delay_ms(uint16_t ctr);
void Delay_xms(uint16_t nms);
void Delay_s(unsigned int ms);
#endif

