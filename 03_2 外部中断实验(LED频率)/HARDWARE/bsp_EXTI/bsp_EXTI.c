#include "bsp_EXTI.h"


/*******************************************************************
功  能：置外部中断  
参  数: 无
返回值: 无
********************************************************************/
void EXTI_Hardware_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;//EXTI结构体变量
	NVIC_InitTypeDef NVIC_InitStruct;//嵌套中断中断控制器结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开时钟

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//配置中断组
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//中断源
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority  = 0;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);//初始化配置
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;//中断源
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority  = 0;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);//初始化配置
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);//中断线关联,PA0,中断线为key1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);//中断线关联,PC13,中断线为key2
	//配置EXTI
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;//PA0,key1
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger  = EXTI_Trigger_Rising;//触发选择（上升沿or下降沿触发中断）
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能中断屏蔽寄存器
	EXTI_Init(&EXTI_InitStruct);//初始化配置
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;//PC13,key2
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger  = EXTI_Trigger_Rising;//触发选择（上升沿or下降沿触发中断）
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能中断屏蔽寄存器
	EXTI_Init(&EXTI_InitStruct);//初始化配置
}


/*******************************************************************
功  能：外部中断0服务函数  
参  数: 无
返回值: 无
********************************************************************/
void EXTI0_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line0) !=RESET)
	{
		time = 100;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志
}


/*******************************************************************
功  能：外部中断13服务函数
参  数: 无
返回值: 无
********************************************************************/
void EXTI15_10_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line13) !=RESET)
	{
		time = 500;
	}
	EXTI_ClearITPendingBit(EXTI_Line13);//清除中断标志
}


/*******************************************************************
功  能：LED频率显示函数
参  数: 无
返回值: 无
********************************************************************/
void LED_Frequency_Display(void)
{
	
	if(time)//出发按键外部中断
	{
		if(time == 100)//按键1被按下
		{
			flag = 1;
		}
		else if(time == 500)//按键2被按下
		{
			flag = 2;
		}
	}
	switch(flag)
	{
		case 1:LED_R(0);Delay_ms(100);LED_R(1);Delay_ms(100);break;
		case 2:LED_G(0);Delay_ms(500);LED_G(1);Delay_ms(500);break;
		default:break;
	}
}

