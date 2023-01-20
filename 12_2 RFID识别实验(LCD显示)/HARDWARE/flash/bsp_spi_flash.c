#include "bsp_spi_flash.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/**
  * @brief  SPI_FLASH��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��SPIʱ�� */
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1, ENABLE );

	/* ʹ��SPI������ص�ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE );

	/* ����SPI�� CS���ţ���ͨIO���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ����SPI�� SCK����*//* ����SPI�� MISO����*//* ����SPI�� MOSI����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
	GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	/* SPI ģʽ���� */
	// FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1 , &SPI_InitStructure);

	/* ʹ�� SPI  */
	SPI_Cmd(SPI1 , ENABLE);
	
}
 /**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
	/* ����FLASHдʹ������ */
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	/* �������� */
	/* ѡ��FLASH: CS�͵�ƽ */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );
	/* ������������ָ��*/
	SPI_FLASH_SendByte(0x20);
	/*���Ͳ���������ַ�ĸ�λ*/
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	/* ���Ͳ���������ַ����λ */
	SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	/* ���Ͳ���������ַ�ĵ�λ */
	SPI_FLASH_SendByte(SectorAddr & 0xFF);
	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );
	/* �ȴ��������*/
	SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_BulkErase(void)
{
	/* ����FLASHдʹ������ */
	SPI_FLASH_WriteEnable();

	/* ���� Erase */
	/* ѡ��FLASH: CS�͵�ƽ */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );
	/* �����������ָ��*/
	SPI_FLASH_SendByte(0xC7);
	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	/* �ȴ��������*/
	SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	/* ����FLASHдʹ������ */
	SPI_FLASH_WriteEnable();

	/* ѡ��FLASH: CS�͵�ƽ */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );
	/* дҳдָ��*/
	SPI_FLASH_SendByte(0x02);
	/*����д��ַ�ĸ�λ*/
	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	/*����д��ַ����λ*/
	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	/*����д��ַ�ĵ�λ*/
	SPI_FLASH_SendByte(WriteAddr & 0xFF);

	if(NumByteToWrite > 256)
	{
		 NumByteToWrite = 256;
		 FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
	}

	/* д������*/
	while (NumByteToWrite--)
	{
		/* ���͵�ǰҪд����ֽ����� */
		SPI_FLASH_SendByte(*pBuffer);
		/* ָ����һ�ֽ����� */
		pBuffer++;
	}

	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	/* �ȴ�д�����*/
	SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % 256;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = 256 - Addr;
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / 256;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % 256;
	
	/* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0)
  {
	/* NumByteToWrite < SPI_FLASH_PageSize */
	if (NumOfPage == 0) 
	{
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
	}
	else /* NumByteToWrite > SPI_FLASH_PageSize */
	{ 
			/*�Ȱ�����ҳ��д��*/
		while (NumOfPage--)
		{
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, 256);
			WriteAddr +=  256;
			pBuffer += 256;
	}
		/*���ж���Ĳ���һҳ�����ݣ�����д��*/
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
	}
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
	else 
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
		/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
	if (NumOfSingle > count) 
	{
		temp = NumOfSingle - count;
				/*��д����ǰҳ*/
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				
		WriteAddr +=  count;
		pBuffer += count;
				/*��дʣ�������*/
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
	else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
		/*��ַ����������count�ֿ������������������*/
		NumByteToWrite -= count;
		NumOfPage =  NumByteToWrite / 256;
		NumOfSingle = NumByteToWrite % 256;

		/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);

		/* ���������ظ���ַ�������� */
		WriteAddr +=  count;
		pBuffer += count;
		/*������ҳ��д��*/
	while (NumOfPage--)
	{
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, 256);
		WriteAddr +=  256;
		pBuffer += 256;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
	if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
		}
	}
}

 /**
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	/* ѡ��FLASH: CS�͵�ƽ */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

	/* ���� �� ָ�� */
	SPI_FLASH_SendByte(0x03);

	/* ���� �� ��ַ��λ */
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* ���� �� ��ַ��λ */
	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
	/* ���� �� ��ַ��λ */
	SPI_FLASH_SendByte(ReadAddr & 0xFF);

	/* ��ȡ���� */
	while (NumByteToRead--) /* while there is data to be read */
	{
		/* ��ȡһ���ֽ�*/
		*pBuffer = SPI_FLASH_SendByte(0xFF);
		/* ָ����һ���ֽڻ����� */
		pBuffer++;
	}

	/* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );
}

 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* ��ʼͨѶ��CS�͵�ƽ */
	GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

	/* ����JEDECָ���ȡID */
	SPI_FLASH_SendByte(0x9F);

	/* ��ȡһ���ֽ����� */
	Temp0 = SPI_FLASH_SendByte(0xFF);

	/* ��ȡһ���ֽ����� */
	Temp1 = SPI_FLASH_SendByte(0xFF);

	/* ��ȡһ���ֽ����� */
	Temp2 = SPI_FLASH_SendByte(0xFF);

	/* ֹͣͨѶ��CS�ߵ�ƽ */
	GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	/*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}
 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
  * @retval FLASH Device ID
  */
u32 SPI_FLASH_ReadDeviceID(void)
{
	u32 Temp = 0;

	/* Select the FLASH: Chip Select low */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

	/* Send "RDID " instruction */
	SPI_FLASH_SendByte(0xAB);
	SPI_FLASH_SendByte(0xFF);
	SPI_FLASH_SendByte(0xFF);
	SPI_FLASH_SendByte(0xFF);

	/* Read a byte from the FLASH */
	Temp = SPI_FLASH_SendByte(0xFF);

	/* Deselect the FLASH: Chip Select high */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
	/* Select the FLASH: Chip Select low */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

	/* Send "Read from Memory " instruction */
	SPI_FLASH_SendByte(0x03);

	/* Send the 24-bit address of the address to read from -----------------------*/
	/* Send ReadAddr high nibble address byte */
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte */
	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte */
	SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


 /**
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
u8 SPI_FLASH_ReadByte(void)
{
	return (SPI_FLASH_SendByte(0xFF));
}

 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(SPI1 , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(SPI1 );
}

 /**
  * @brief  ʹ��SPI���������ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }
	
  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(SPI1 , HalfWord);

	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(SPI1 );
}

 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
  /* ͨѶ��ʼ��CS�� */
   GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

  /* ����дʹ������*/
  SPI_FLASH_SendByte(0x06);

  /*ͨѶ������CS�� */
  GPIO_SetBits( GPIOC, GPIO_Pin_0);
}

/* WIP(busy)��־��FLASH�ڲ�����д�� */

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* ѡ�� FLASH: CS �� */
   GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

  /* ���� ��״̬�Ĵ��� ���� */
  SPI_FLASH_SendByte(0x05);

  /* ��FLASHæµ����ȴ� */
  do
  {
		/* ��ȡFLASHоƬ��״̬�Ĵ��� */
    FLASH_Status = SPI_FLASH_SendByte(0xFF);	 
  }
  while ((FLASH_Status & 0x01) == SET);  /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
  GPIO_SetBits( GPIOC, GPIO_Pin_0);
}


//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  /* ͨѶ��ʼ��CS�� */
   GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

  /* ���� ���� ���� */
  SPI_FLASH_SendByte(0xB9);

  /*ͨѶ������CS�� */
  GPIO_SetBits( GPIOC, GPIO_Pin_0);
}   

//����
void SPI_Flash_WAKEUP(void)   
{
  /*ѡ�� FLASH: CS �� */
  GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

  /* ���� �ϵ� ���� */
  SPI_FLASH_SendByte(0xAB);

   /* ֹͣ�ź� FLASH: CS �� */
  GPIO_SetBits( GPIOC, GPIO_Pin_0);
}   
   

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}
   
/*********************************************END OF FILE**********************/

