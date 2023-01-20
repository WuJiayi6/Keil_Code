#ifndef __KEY_H_
#define __KEY_H_

#include "sys.h"
#include "led.h" 
#include "delay.h" 
 
#define KEY1() 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) 
#define KEY2() 	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)	

void Key_Hardware_Init(void);	    //IO初始化
uint8_t Key_Scan(uint8_t);  		//按键扫描函数

#endif
