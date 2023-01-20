#ifndef __LED_H_
#define __LED_H_

#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"

#define LED_G(X) GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)X)	// LED_G
#define LED_B(X) GPIO_WriteBit(GPIOB,GPIO_Pin_1,(BitAction)X)	// LED_B	 
#define LED_R(X) GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)X)	// LED_R
#define LED_Y(X) {GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)X); \
				  GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)X);}// LED_Y
#define LED_C(X) {GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)X); \
				  GPIO_WriteBit(GPIOB,GPIO_Pin_1,(BitAction)X);}// LED_C   cyanÇàÉ«
#define LED_P(X) {GPIO_WriteBit(GPIOB,GPIO_Pin_1,(BitAction)X); \
				  GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)X);}// LED_P 
#define LED_W(X) {GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)X); \
				  GPIO_WriteBit(GPIOB,GPIO_Pin_1,(BitAction)X); \
				  GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)X);}// LED_W	


#define LEDG_TOGGLE()	GPIO_ToggleBits(GPIOB,GPIO_Pin_0)
#define LEDB_TOGGLE()	{GPIOB -> ODR ^= GPIO_Pin_1;}//²Ù¿ØODR¼Ä´æÆ÷
#define LEDR_TOGGLE()	GPIO_ToggleBits(GPIOB,GPIO_Pin_5)

#define Beep_ON         GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define Beep_OFF        GPIO_ResetBits(GPIOA,GPIO_Pin_8)

void LED_Hardware_Init(void);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
#endif
