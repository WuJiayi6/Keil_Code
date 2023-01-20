#include "SPI.h"
/*******************************************************************
��  �ܣ�SPI1��ʼ��  
��  ��: ��
����ֵ: ��
********************************************************************/
void SPI1_Hardware_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
                           RCC_APB2Periph_GPIOB,ENABLE);//PORTA��Bʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );//SPI1ʱ��ʹ��

    /********** CS Ƭѡ **********/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /********* SCK ʱ�� **********/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /********* MISO ���մӷ� ******/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /******** MOSI �������� *******/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /********  RST ��λ ***********/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /********* ���� SPI ***********/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����SPIȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//��Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//SPI�շ�8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//����ͬ��ʱ�ӵĵڶ�������������(����)������
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�ź����������
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//��������36M/8=4.5MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;//CRCֵ����Ķ���ʽ
    SPI_Init(SPI1,&SPI_InitStructure); 

    SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
}


/*******************************************************************
��  �ܣ�RC522ͨ��SPI����  
��������SPI_RC522_SendByte
��  ��: byte,Ҫ���͵�����
����ֵ: ��
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
��  �ܣ�RC522ͨ��SPI��ȡ 
��������SPI_RC522_ReadByte
��  ��: byte,Ҫ���͵�����
����ֵ: SPI_Data,RC522���ص�����
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

