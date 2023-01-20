#include "stm32f10x.h"
#include "delay.h"

static uint32_t fam_nus = 0;
static uint32_t fam_nms = 0;

/**********************************************************
功  能：延时初始化
参  数：无
返回值：无
**********************************************************/
void Delay_Init(void)
{                                                            
    //关闭计时、不触发中断、计数时钟为AHB/8
    SysTick->CTRL = 0x0;
    //微秒级初值
    fam_nus = SystemCoreClock/7200000;
    //毫秒级初值
    fam_nms = fam_nus * 1000;
    //计数器清零
    SysTick->VAL = 0;
}

/**********************************************************
功  能：微秒级延时
参  数：ctr 微秒倍数、例如 ctr = 1 则 延时1us
返回值：无
**********************************************************/
void Delay_us(uint16_t ctr)
{
    uint32_t tick_flag = 0;
    
    //定时器初值
    SysTick->LOAD = ctr*fam_nus;   
    //计数器清零
    SysTick->VAL = 0;
    //开启定时器
    SysTick->CTRL |= 0x01;
    do
    {
        //获取定时器状态
        tick_flag = SysTick->CTRL;
    }
    //状态判断
    while(!(tick_flag&(1<<16)) && (tick_flag&0x01));
    //关闭计时
    SysTick->CTRL = 0x00;
}

/**********************************************************
功  能：毫秒级延时
参  数：ctr 微秒倍数、例如 ctr = 1 则 延时1ms
返回值：无
**********************************************************/
void Delay_ms(uint16_t ctr)
{
    uint32_t tick_flag = 0;
    
    SysTick->LOAD = ctr * fam_nms;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x01;
    do
    {
        tick_flag = SysTick->CTRL;
    }
    while(!(tick_flag & (1<<16)) && (tick_flag & 0x01));
    SysTick->CTRL = 0x00;
}

/**********************************************************
功  能：延时nms
参  数：ctr 微秒倍数、例如 ctr = 1 则 延时1ms
返回值：无
**********************************************************/
void Delay_xms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fam_nms;						//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           						//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  //关闭计数器
	SysTick->VAL =0X00;     		  						//清空计数器	  	    
} 

