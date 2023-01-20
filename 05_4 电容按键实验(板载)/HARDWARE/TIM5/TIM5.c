#include "TIM5.h"
 
/*******************************************************************
功  能：通用定时器5中断初始化  
参  数: arr：自动重装值
		psc：时钟预分频数
		定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
		Ft=定时器工作频率,单位:Mhz
返回值: 无
********************************************************************/

void TIM5_Hardware_Init(uint16_t arr,uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	//使能GPIOA和TIM5的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//将引脚拉低
	
	//时基初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStruct.TIM_Period=arr-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	
	//输入捕获初始化
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;//PA1-->TIM5_CH2
	TIM_ICInitStruct.TIM_ICFilter=0x3;//2^3=8倍滤波
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;//不分频捕获
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;//直接映射到TI2
	TIM_ICInit(TIM5,&TIM_ICInitStruct);//TIM5通道2初始化
	
	TIM_Cmd(TIM5,ENABLE);//使能定时器5
}
