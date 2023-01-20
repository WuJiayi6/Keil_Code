#include "bsp_spi_flash.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/**
  * @brief  SPI_FLASH初始化
  * @param  无
  * @retval 无
  */
void SPI_FLASH_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能SPI时钟 */
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1, ENABLE );

	/* 使能SPI引脚相关的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE );

	/* 配置SPI的 CS引脚，普通IO即可 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* 配置SPI的 SCK引脚*//* 配置SPI的 MISO引脚*//* 配置SPI的 MOSI引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 停止信号 FLASH: CS引脚高电平*/
	GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	/* SPI 模式配置 */
	// FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
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

	/* 使能 SPI  */
	SPI_Cmd(SPI1 , ENABLE);
	
}
 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
	/* 发送FLASH写使能命令 */
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	/* 擦除扇区 */
	/* 选择FLASH: CS低电平 */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );
	/* 发送扇区擦除指令*/
	SPI_FLASH_SendByte(0x20);
	/*发送擦除扇区地址的高位*/
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	/* 发送擦除扇区地址的中位 */
	SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	/* 发送擦除扇区地址的低位 */
	SPI_FLASH_SendByte(SectorAddr & 0xFF);
	/* 停止信号 FLASH: CS 高电平 */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );
	/* 等待擦除完毕*/
	SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{
	/* 发送FLASH写使能命令 */
	SPI_FLASH_WriteEnable();

	/* 整块 Erase */
	/* 选择FLASH: CS低电平 */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );
	/* 发送整块擦除指令*/
	SPI_FLASH_SendByte(0xC7);
	/* 停止信号 FLASH: CS 高电平 */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	/* 等待擦除完毕*/
	SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	/* 发送FLASH写使能命令 */
	SPI_FLASH_WriteEnable();

	/* 选择FLASH: CS低电平 */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );
	/* 写页写指令*/
	SPI_FLASH_SendByte(0x02);
	/*发送写地址的高位*/
	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	/*发送写地址的中位*/
	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	/*发送写地址的低位*/
	SPI_FLASH_SendByte(WriteAddr & 0xFF);

	if(NumByteToWrite > 256)
	{
		 NumByteToWrite = 256;
		 FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
	}

	/* 写入数据*/
	while (NumByteToWrite--)
	{
		/* 发送当前要写入的字节数据 */
		SPI_FLASH_SendByte(*pBuffer);
		/* 指向下一字节数据 */
		pBuffer++;
	}

	/* 停止信号 FLASH: CS 高电平 */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	/* 等待写入完毕*/
	SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % 256;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = 256 - Addr;
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / 256;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % 256;
	
	/* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0)
  {
	/* NumByteToWrite < SPI_FLASH_PageSize */
	if (NumOfPage == 0) 
	{
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
	}
	else /* NumByteToWrite > SPI_FLASH_PageSize */
	{ 
			/*先把整数页都写了*/
		while (NumOfPage--)
		{
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, 256);
			WriteAddr +=  256;
			pBuffer += 256;
	}
		/*若有多余的不满一页的数据，把它写完*/
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
	}
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
	else 
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
		/*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
	if (NumOfSingle > count) 
	{
		temp = NumOfSingle - count;
				/*先写满当前页*/
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				
		WriteAddr +=  count;
		pBuffer += count;
				/*再写剩余的数据*/
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
	else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
		/*地址不对齐多出的count分开处理，不加入这个运算*/
		NumByteToWrite -= count;
		NumOfPage =  NumByteToWrite / 256;
		NumOfSingle = NumByteToWrite % 256;

		/* 先写完count个数据，为的是让下一次要写的地址对齐 */
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);

		/* 接下来就重复地址对齐的情况 */
		WriteAddr +=  count;
		pBuffer += count;
		/*把整数页都写了*/
	while (NumOfPage--)
	{
		SPI_FLASH_PageWrite(pBuffer, WriteAddr, 256);
		WriteAddr +=  256;
		pBuffer += 256;
      }
			/*若有多余的不满一页的数据，把它写完*/
	if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
		}
	}
}

 /**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	/* 选择FLASH: CS低电平 */
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

	/* 发送 读 指令 */
	SPI_FLASH_SendByte(0x03);

	/* 发送 读 地址高位 */
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* 发送 读 地址中位 */
	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
	/* 发送 读 地址低位 */
	SPI_FLASH_SendByte(ReadAddr & 0xFF);

	/* 读取数据 */
	while (NumByteToRead--) /* while there is data to be read */
	{
		/* 读取一个字节*/
		*pBuffer = SPI_FLASH_SendByte(0xFF);
		/* 指向下一个字节缓冲区 */
		pBuffer++;
	}

	/* 停止信号 FLASH: CS 高电平 */
	 GPIO_SetBits( GPIOC, GPIO_Pin_0 );
}

 /**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* 开始通讯：CS低电平 */
	GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

	/* 发送JEDEC指令，读取ID */
	SPI_FLASH_SendByte(0x9F);

	/* 读取一个字节数据 */
	Temp0 = SPI_FLASH_SendByte(0xFF);

	/* 读取一个字节数据 */
	Temp1 = SPI_FLASH_SendByte(0xFF);

	/* 读取一个字节数据 */
	Temp2 = SPI_FLASH_SendByte(0xFF);

	/* 停止通讯：CS高电平 */
	GPIO_SetBits( GPIOC, GPIO_Pin_0 );

	/*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}
 /**
  * @brief  读取FLASH Device ID
  * @param 	无
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
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
u8 SPI_FLASH_ReadByte(void)
{
	return (SPI_FLASH_SendByte(0xFF));
}

 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(SPI1 , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return SPI_I2S_ReceiveData(SPI1 );
}

 /**
  * @brief  使用SPI发送两个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }
	
  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(SPI1 , HalfWord);

	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* 读取数据寄存器，获取接收缓冲区数据 */
  return SPI_I2S_ReceiveData(SPI1 );
}

 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
  /* 通讯开始：CS低 */
   GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

  /* 发送写使能命令*/
  SPI_FLASH_SendByte(0x06);

  /*通讯结束：CS高 */
  GPIO_SetBits( GPIOC, GPIO_Pin_0);
}

/* WIP(busy)标志，FLASH内部正在写入 */

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* 选择 FLASH: CS 低 */
   GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

  /* 发送 读状态寄存器 命令 */
  SPI_FLASH_SendByte(0x05);

  /* 若FLASH忙碌，则等待 */
  do
  {
		/* 读取FLASH芯片的状态寄存器 */
    FLASH_Status = SPI_FLASH_SendByte(0xFF);	 
  }
  while ((FLASH_Status & 0x01) == SET);  /* 正在写入标志 */

  /* 停止信号  FLASH: CS 高 */
  GPIO_SetBits( GPIOC, GPIO_Pin_0);
}


//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  /* 通讯开始：CS低 */
   GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

  /* 发送 掉电 命令 */
  SPI_FLASH_SendByte(0xB9);

  /*通讯结束：CS高 */
  GPIO_SetBits( GPIOC, GPIO_Pin_0);
}   

//唤醒
void SPI_Flash_WAKEUP(void)   
{
  /*选择 FLASH: CS 低 */
  GPIO_ResetBits( GPIOC, GPIO_Pin_0 );

  /* 发送 上电 命令 */
  SPI_FLASH_SendByte(0xAB);

   /* 停止信号 FLASH: CS 高 */
  GPIO_SetBits( GPIOC, GPIO_Pin_0);
}   
   

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}
   
/*********************************************END OF FILE**********************/

