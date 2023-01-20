//=======================================Һ���������߽���==========================================//
//   D1          ��          PB15        //OLED��SPIд�ź�
//   CS          ��          PB12        //OLED��Ƭѡ�����ź�
//   RES         ��          PC6        //OLED����λ�����ź�
//   DC          ��          PC7        //OLED������/����ѡ������ź�
//   D0          ��          PB13        //OLED��SPIʱ���ź�
//=========================================����������=========================================//

	
#include "SPI2.h"


/*****************************************************************************
 * @name       :uint8_t SPI_WriteByte(uint8_t Byte)
 * @function   :ʹ��STM32��Ӳ��SPIд��һ���ֽڵ�����
 * @parameters :Byte:Ҫд�������
 * @retvalue   :���߽��յ�����
******************************************************************************/
uint8_t SPI_WriteByte(uint8_t Byte)
{
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==RESET);		//�ȴ���������	  
	SPI2->DR=Byte;	 	//����һ��byte   
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==RESET);//�ȴ�������һ��byte  
	return SPI2->DR;          	     //�����յ�������			
}

/*****************************************************************************
 * @name       :void SPI_SetSpeed(uint8_t SpeedSet)
 * @function   :����Ӳ��SPI�ٶ�
 * @parameters :SpeedSet:0-high speed     1-low speed
 * @retvalue   :��
******************************************************************************/
void SPI_SetSpeed(uint8_t SpeedSet)
{
	SPI2->CR1&=0XFFC7;
	if(SpeedSet==1)//����
	{
		SPI2->CR1|=SPI_BaudRatePrescaler_2;//Fsck=Fpclk/2	
	}
	else//����
	{
		SPI2->CR1|=SPI_BaudRatePrescaler_32; //Fsck=Fpclk/32
	}
	SPI2->CR1|=1<<6; //SPI�豸ʹ��
} 

/*****************************************************************************
 * @name       :void SPI2_Init(void)	
 * @function   :��ʼ��STM32Ӳ��SPI2
 * @parameters :��
 * @retvalue   :��
******************************************************************************/
void SPI2_Hardware_Init(void)	
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	//����SPI2�ܽ�
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
	
	//SPI2����ѡ��
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

	//ʹ��SPI2
	SPI_Cmd(SPI2, ENABLE);   
}

