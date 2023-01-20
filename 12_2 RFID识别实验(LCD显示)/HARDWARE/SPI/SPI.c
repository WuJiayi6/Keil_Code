#include "SPI.h"
/*******************************************************************
功  能：SPI1初始化  
参  数: 无
返回值: 无
********************************************************************/
void SPI1_Hardware_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
                           RCC_APB2Periph_GPIOB,ENABLE);//PORTA、B时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );//SPI1时钟使能

    /********** CS 片选 **********/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /********* SCK 时钟 **********/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /********* MISO 主收从发 ******/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /******** MOSI 主发从收 *******/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /********  RST 复位 ***********/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /********* 配置 SPI ***********/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//设置SPI全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//SPI收发8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//串行同步时钟的第二个跳变沿上升(数据)被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由软件管理
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//传输速率36M/8=4.5MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC值计算的多项式
    SPI_Init(SPI1,&SPI_InitStructure); 

    SPI_Cmd(SPI1, ENABLE); //使能SPI外设
}


/*******************************************************************
功  能：RC522通过SPI发送  
函数名：SPI_RC522_SendByte
参  数: byte,要发送的数据
返回值: 无
********************************************************************/
void SPI_RC522_SendByte(uint8_t byte)
{
    uint8_t counter;
    for(counter=0; counter<8; counter++)
    {
        if ( byte & 0x80 )
            RC522_MOSI_1();
        else
            RC522_MOSI_0();

        Delay_us(20);
        RC522_SCK_0();
        Delay_us(20);
        RC522_SCK_1();
        Delay_us(20);
		  
        byte <<= 1;
    }
}


/*******************************************************************
功  能：RC522通过SPI读取 
函数名：SPI_RC522_ReadByte
参  数: byte,要发送的数据
返回值: SPI_Data,RC522返回的数据
********************************************************************/
uint8_t SPI_RC522_ReadByte(void)
{
    uint8_t counter;
    uint8_t SPI_Data;

    for(counter=0; counter<8; counter++)
    {
        SPI_Data <<= 1;
        RC522_SCK_0();
        Delay_us(20);

        if (RC522_MISO_GET() == 1)
        {SPI_Data |= 0x01;}

        Delay_us(20);
        RC522_SCK_1();
        Delay_us(20);
    }

    return SPI_Data;
}

