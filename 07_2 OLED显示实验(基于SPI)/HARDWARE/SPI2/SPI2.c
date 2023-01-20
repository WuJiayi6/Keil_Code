//=======================================液晶屏数据线接线==========================================//
//   D1          接          PB15        //OLED屏SPI写信号
//   CS          接          PB12        //OLED屏片选控制信号
//   RES         接          PC6        //OLED屏复位控制信号
//   DC          接          PC7        //OLED屏数据/命令选择控制信号
//   D0          接          PB13        //OLED屏SPI时钟信号
//=========================================触摸屏接线=========================================//

	
#include "SPI2.h"


/*****************************************************************************
 * @name       :uint8_t SPI_WriteByte(uint8_t Byte)
 * @function   :使用STM32的硬件SPI写入一个字节的数据
 * @parameters :Byte:要写入的数据
 * @retvalue   :总线接收的数据
******************************************************************************/
uint8_t SPI_WriteByte(uint8_t Byte)
{
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPI2->DR=Byte;	 	//发送一个byte   
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完一个byte  
	return SPI2->DR;          	     //返回收到的数据			
}

/*****************************************************************************
 * @name       :void SPI_SetSpeed(uint8_t SpeedSet)
 * @function   :设置硬件SPI速度
 * @parameters :SpeedSet:0-high speed     1-low speed
 * @retvalue   :无
******************************************************************************/
void SPI_SetSpeed(uint8_t SpeedSet)
{
	SPI2->CR1&=0XFFC7;
	if(SpeedSet==1)//高速
	{
		SPI2->CR1|=SPI_BaudRatePrescaler_2;//Fsck=Fpclk/2	
	}
	else//低速
	{
		SPI2->CR1|=SPI_BaudRatePrescaler_32; //Fsck=Fpclk/32
	}
	SPI2->CR1|=1<<6; //SPI设备使能
} 

/*****************************************************************************
 * @name       :void SPI2_Init(void)	
 * @function   :初始化STM32硬件SPI2
 * @parameters :无
 * @retvalue   :无
******************************************************************************/
void SPI2_Hardware_Init(void)	
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	//配置SPI2管脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	/************    SCLK   ******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/************    MISO   ******************/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/************    MOSI   ******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//SPI2配置选项
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
	   
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	//使能SPI2
	SPI_Cmd(SPI2, ENABLE);   
}

