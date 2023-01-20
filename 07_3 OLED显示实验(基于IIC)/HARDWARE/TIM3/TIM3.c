#include "TIM3.h"
/*******************************************************************
功  能：通用定时器3中断初始化  
参  数: arr：自动重装值
		psc：时钟预分频数
		定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
		Ft=定时器工作频率,单位:Mhz
返回值: 无
********************************************************************/
void TIM3_Hardware_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);				//使能TIM3时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//初始化TIM3

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);						//允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE);	//使能定时器3

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;					//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;		//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;				//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************
功  能：定时器中断函数 
参  数: 无
返回值: 无
********************************************************************/
void TIM3_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 	//溢出中断
	{
		flag_TIM3 =~flag_TIM3;
		LEDR_TOGGLE();
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  	//清除中断标志位
}


/*********************************************************************
	                串口显示24小时时钟
**********************************************************************/
void TIM3_24_Clock(void)
{

	if(flag_TIM3)
	{
		s++;
		if(s==60){m++;s=0;}
		if(m==60){h++;m=0;}
		h%=24;//小时到24就清零
		flag_TIM3 =~flag_TIM3;

	}
}
