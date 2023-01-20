#ifndef __TPAD_H_
#define __TPAD_H_


#include "sys.h"
#include "led.h"
#include "stm32f10x.h"

#define MAX_ARR_VAL 0XFFFF		//设置最大采样值
#define GATE_VAL 20				//门限值

static uint16_t DefaultVal;//默认的采样时间

void TPAD_Init(void);
uint16_t TPAD_Get_VAL(void);
void TPAD_Reset(void);
uint16_t TPAD_Get_MaxVal(uint8_t sample);
uint8_t TPAD_Scan(uint8_t mode);


#endif
