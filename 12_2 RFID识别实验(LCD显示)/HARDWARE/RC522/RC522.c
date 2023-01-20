#include "RC522.h"

//////////////////////////////////////////////////////////
// M1卡分为16个扇区，每个扇区由四个块（块0、块1、块2、块3）组成
// 将16个扇区的64个块按绝对地址编号为：0~63
// 第0个扇区的块0（即绝对地址0块），用于存放厂商代码，已经固化不可更改
// 每个扇区的块0、块1、块2为数据块，可用于存放数据
// 每个扇区的块3为控制块（绝对地址为:块3、块7、块11.....）包括密码A，存取控制、密码B等

/*******************************
*连线说明：
*1--SDA  <----->PA4
*2--SCK  <----->PA5
*3--MOSI <----->PA7
*4--MISO <----->PA6
*5--悬空
*6--GND <----->GND
*7--RST <----->PB0
*8--VCC <----->VCC
************************************/
/*全局变量*/
unsigned char CT[2];//卡类型
unsigned char SN[4]; //卡号
unsigned char DATA[16];//存放数据 
uint8_t KEY_A[6]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//密钥
unsigned char DATA1[16]= {'W','J','Y','2','0','7','1','0','2','2','5'};
unsigned char STATUS;
unsigned char Block_Addr=0x08;// 0x08 就是2扇区0区块（即第9块）
char LCD_Show[50] = {0};
char LCD_read[50] = {0};
char LCD_wirte[50] = {0};
uint8_t LCD_flag = 0;


/*******************************************************************
功  能： RC522初始化
函数名： RC522_Init
参  数： 无
返回值： 无
********************************************************************/
void RC522_Init(void)
{
    SPI1_Hardware_Init();//SPI1初始化
	RC522_Reset_Disable();	
	RC522_CS_Disable();    
    PcdReset();//RC522复位
    M500PcdConfigISOType('A');//设置工作方式

}


/*******************************************************************
功  能： 读RC522寄存器
函数名： ReadRawRC
参  数： ucAddress,寄存器地址
返回值： ucReturn,寄存器的当前值
********************************************************************/
uint8_t ReadRawRC(uint8_t ucAddress)
{
    uint8_t ucAddr, ucReturn;
    ucAddr = ((ucAddress << 1 ) & 0x7E ) | 0x80;
    RC522_CS_Enable();
    SPI_RC522_SendByte(ucAddr);
    ucReturn = SPI_RC522_ReadByte();
    RC522_CS_Disable();

    return ucReturn;
}


/**************************************************************
*功  能：写RC522寄存器
*函数名：WriteRawRC
*参  数: ucAddress,寄存器地址  ucValue,写入寄存器的值
*返回值: 无
**************************************************************/
void WriteRawRC(uint8_t ucAddress,uint8_t ucValue)
{
    uint8_t ucAddr;
    ucAddr = (ucAddress << 1) & 0x7E;
    RC522_CS_Enable();
    SPI_RC522_SendByte(ucAddr);
    SPI_RC522_SendByte(ucValue);
    RC522_CS_Disable();
}


/**************************************************************
*功  能：对RC522寄存器置位
*函数名：SetBitMask
*参  数: reg[IN]:寄存器地址  mask[IN]:置位值
*返回值: 无
**************************************************************/
void SetBitMask(uint8_t reg,uint8_t mask)
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}


/**************************************************************
*功  能：对RC522寄存器清位
*函数名：ClearBitMask
*参  数: reg[IN]:寄存器地址  mask[IN]:清位值
*返回值: 无
**************************************************************/
void ClearBitMask(uint8_t reg,uint8_t mask)
{
    uint8_t ucTemp;
    ucTemp = ReadRawRC(reg);
    WriteRawRC(reg, ucTemp & (~ mask));// clear bit mask
}


/**************************************************************
*功  能：开启天线（天线开启或关闭间隔至少1ms）
*函数名：PcdAntennaOn
*参  数: 无
*返回值: 无
**************************************************************/
void PcdAntennaOn(void)
{
    uint8_t i;
    i = ReadRawRC ( TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }   
}



/**************************************************************
*功  能：关闭天线
*函数名：PcdAntennaOff
*参  数: 无
*返回值: 无
**************************************************************/
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg,0x03);
}


/*******************************************************************
功  能：复位RC522 
函数名：PcdReset
参  数: 无
返回值: 成功返回MI_OK
********************************************************************/
void PcdReset(void)
{
	RC522_Reset_Disable();
	Delay_us(1);
	RC522_Reset_Enable();
	Delay_us(1);
	RC522_Reset_Disable();
	Delay_us(1);
    WriteRawRC(CommandReg,0x0F);
    while(ReadRawRC(CommandReg) & 0x10);
    Delay_us(1);
    WriteRawRC(ModeReg,0x3D);//和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);//16位定时器低位
    WriteRawRC(TReloadRegH,0);//16位定时器高位
    WriteRawRC(TModeReg, 0x8D);//定义内部定时器的设置
    WriteRawRC(TPrescalerReg, 0x3E );//设置定时器分频系数
    WriteRawRC(TxAutoReg, 0x40 );//调制发送信号为100%ASK
    
}

/*******************************************************************
* 功  能：设置RC522的工作方式 
* 函数名：M500PcdConfigISOType
* 参  数: ucType,工作方式
* 返回值: 无
********************************************************************/
void M500PcdConfigISOType(uint8_t ucType)
{
    if ( ucType == 'A')//ISO14443_A
    {
        ClearBitMask(Status2Reg,0x08);
        WriteRawRC(ModeReg,0x3D);//3D
        WriteRawRC(RxSelReg,0x86);//86
        WriteRawRC(RFCfgReg,0x7F);//7F
        WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec)
        WriteRawRC(TReloadRegH,0);
        WriteRawRC(TModeReg,0x8D);
        WriteRawRC(TPrescalerReg,0x3E);
        Delay_us(2);
        PcdAntennaOn();//开天线
    }
}


/**************************************************************
*功  能：通过RC522和ISO14443卡通讯
*函数名：PcdComMF522
*参  数: Command[IN]:RC522命令字
*		 pInData[IN]:通过RC522发送到卡片的数据
*		 InLenByte[IN]:发送数据的字节长度
*		 pOutData[OUT]:接收到的卡片返回数据
*		 *pOutLenBit[OUT]:返回数据的位长度
*返回值: 无
**************************************************************/
char PcdComMF522(uint8_t Command,
                 uint8_t *pIn,
                 uint8_t InLenByte,
                 uint8_t *pOut,
                 uint8_t *pOutLenBit)
{
    char   status = MI_ERR;
    uint8_t   irqEn   = 0x00;
    uint8_t   waitFor = 0x00;
    uint8_t   lastBits;
    uint8_t   n;
    uint16_t   i;
    switch (Command)
    {
    case PCD_AUTHENT:       //Mifare认证
        irqEn   = 0x12;    	//允许错误中断请求ErrIEn  允许空闲中断IdleIEn
        waitFor = 0x10;    	//认证寻卡等待时候 查询空闲中断标志位
        break;             
    case PCD_TRANSCEIVE:    //接收发送 发送接收
        irqEn   = 0x77;    	//允许TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
        waitFor = 0x30;    	//寻卡等待时候 查询接收中断标志位与 空闲中断标志位
        break;             	
    default:
        break;
    }
    WriteRawRC(ComIEnReg,irqEn|0x80); //IRqInv置位管脚IRQ与Status1Reg的IRq位的值相反
    ClearBitMask(ComIrqReg,0x80);	  //Set1该位清零时，CommIRqReg的屏蔽位清零
    WriteRawRC(CommandReg,PCD_IDLE);  //写空闲命令
    SetBitMask(FIFOLevelReg,0x80);    //置位FlushBuffer清除内部FIFO的读和写指针以及ErrReg的BufferOvfl标志位被清除

    for (i=0; i<InLenByte; i++)
    {
        WriteRawRC(FIFODataReg, pIn [i]);    //写数据进FIFOdata
    }
    WriteRawRC(CommandReg, Command);//写命令
    if (Command == PCD_TRANSCEIVE)
    {
        SetBitMask(BitFramingReg,0x80);//StartSend置位启动数据发送 该位与收发命令使用时才有效
    }
    i = 1000;//根据时钟频率调整，操作M1卡最大等待时间25ms
    
    do                 //认证与寻卡等待时间
    {
        n = ReadRawRC(ComIrqReg);    //查询事件中断
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));//退出条件i=0,定时器中断与写空闲命令
    ClearBitMask(BitFramingReg,0x80);//清理允许StartSend位

    if (i!=0)
    {
        if(!(ReadRawRC(ErrorReg)&0x1B))//读错误标志寄存器BufferOfI CollErr ParityErr ProtocolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)  //是否发生定时器中断
            {
                status = MI_NOTAGERR;
            }
            if (Command == PCD_TRANSCEIVE)
            {
                n = ReadRawRC(FIFOLevelReg);            //读FIFO中保存的字节数
                lastBits = ReadRawRC(ControlReg) & 0x07;//最后接收到得字节的有效位数
                if (lastBits) 
                {   
                    //N个字节数减去1（最后一个字节）+最后一位的位数 读取到的数据总位数                                       
                    *pOutLenBit = (n-1)*8 + lastBits;
                }                                       	
                else                                    
                {                                       
                    *pOutLenBit = n*8; //最后接收到的字节整个字节有效
                }
                if (n == 0)
                {
                    n = 1;
                }
                if (n > MAXRLEN)
                {
                    n = MAXRLEN;
                }
                for (i=0; i<n; i++)
                {
                    pOut [i] = ReadRawRC(FIFODataReg);
                }
            }
        }
        else
        {
            status = MI_ERR;
        }
    }
    SetBitMask(ControlReg,0x80);           // stop timer now
    WriteRawRC(CommandReg,PCD_IDLE);
    return status;
}


/**************************************************************
*功  能：寻卡
*参  数: req_code[IN]:寻卡方式
*		 0x52 = 寻感应区内所有符合14443A标准的卡
*		 0x26 = 寻未进入休眠状态的卡
*		 pTagType[OUT]：卡片类型代码
*		 0x4400 = Mifare_UltraLight
*		 0x0400 = Mifare_One(S50)
*		 0x0200 = Mifare_One(S70)
*		 0x0800 = Mifare_Pro(X)
*		 0x4403 = Mifare_DESFire
*返回值: 成功返回MI_OK
**************************************************************/
char PcdRequest(uint8_t req_code,uint8_t *pTagType)
{
    char   status;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg,0x08);//清理指示MIFARECyptol单元接通以及所有卡的数据通信被加密的情况
    WriteRawRC(BitFramingReg,0x07);//发送的最后一个字节的七位
    SetBitMask(TxControlReg,0x03);//TX1,TX2管脚的输出信号传递经发送调制的13.56的能量载波信号
    
    ucComMF522Buf[0] = req_code;//存入卡片命令字
    
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x10))//寻卡成功返回卡类型
    {
        *pTagType     = ucComMF522Buf[0];
        *(pTagType+1) = ucComMF522Buf[1];
    }
    else
    {
        status = MI_ERR;
    }
    return status;
}


/**************************************************************
*功  能：防冲撞
*函数名：PcdAnticoll
*参  数: pSnr，卡片序列号，4字节
*返回值: 成功返回MI_OK
**************************************************************/
char PcdAnticoll(uint8_t *pSnr)
{
    char   status;
    uint8_t   i,snr_check=0;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg,0x08);//清MFCryptol On位 只有成功执行MFAuthent命令后，该位才能置位
    WriteRawRC(BitFramingReg,0x00);//清理寄存器 停止收发
    ClearBitMask(CollReg,0x80);//清ValuesAfterColl所有接收的位在冲突后被清除

/*---------------------------------------------------------------- 
    参考ISO14443协议：
    https://blog.csdn.net/wowocpp/article/details/79910800
    PCD 发送 SEL = ‘93’，NVB = ‘20’两个字节 
    迫使所有的在场的PICC发回完整的UID CLn作为应答。
------------------------------------------------------------------*/
    ucComMF522Buf[0] = 0x93;//卡片防冲突命令
    ucComMF522Buf[1] = 0x20;

	 //发送并接收数据 接收的数据存储于ucComMF522Buf
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

/** 与卡片通信 **/
    if (status == MI_OK)//通信成功
    {
		 //收到的UID 存入pSnr
        for (i=0; i<4; i++)
        {
            *(pSnr+i)  = ucComMF522Buf[i];//读出UID
            snr_check ^= ucComMF522Buf[i];
        }
        if (snr_check != ucComMF522Buf[i])
        {
           status = MI_ERR; 
        }
    }

    SetBitMask(CollReg, 0x80);
    return status;
}


/**************************************************************
*功  能：用RC522计算CRC16函数
*函数名：CalulateCRC
*参  数: pIn,计算CRC16的数组   
         len,计算CRC16的数组字节长度
         pOut,存放计算结果存放的首地址
*返回值: 无
**************************************************************/
void CalulateCRC(uint8_t *pIn,uint8_t len,uint8_t *pOut)
{
    uint8_t   i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {
        WriteRawRC(FIFODataReg, *(pIn +i));
    }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOut [0] = ReadRawRC(CRCResultRegL);
    pOut [1] = ReadRawRC(CRCResultRegM);
}


/**************************************************************
*功  能：选定卡片
*函数名：PcdSelect
*参  数: pSnr[IN]:卡片序列号，4字节
*返回值: 成功返回MI_OK
**************************************************************/
char PcdSelect(uint8_t *pSnr)
{
    char   status;
    uint8_t   i;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_ANTICOLL1;//防冲撞 0x93
    
/* 假设没有冲突，PCD 指定NVB为0x70，
 * 此值表示PCD将发送完整的UID CLn，
 * 与40位UID CLn 匹配的PICC，以SAK作为应答*/
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)//3 4 5 6位存放UID，第7位一直异或
    {
        ucComMF522Buf[i+2] = *(pSnr+i);
        ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);//CRC(循环冗余校验)
    ClearBitMask(Status2Reg,0x08);
    /*发送并接收数据*/
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);

    if ((status == MI_OK) && (unLen == 0x18))
    {
        status = MI_OK;
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}


/**************************************************************
*功  能：验证卡片密码
*函数名：PcdAuthState
*参  数: auth_mode[IN]: 
         密码验证模式 0x60 = 验证A密钥 0x61 = 验证B密钥
		 addr[IN]：块地址   
         pKey[IN]：密码  
         pSnr[IN]：卡片序列号，4字节
*返回值: 成功返回MI_OK
**************************************************************/
char PcdAuthState(uint8_t auth_mode,uint8_t addr,uint8_t *pKey,uint8_t *pSnr)
{
    char   status;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
#if 0
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
#endif
/* C 库函数 
   void *memcpy(void *str1, const void *str2, size_t n) 
   从存储区 str2 复制 n 个字节到存储区 str1 */
    memcpy(&ucComMF522Buf[2], pKey, 6);
    memcpy(&ucComMF522Buf[8], pSnr, 4);
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {
        status = MI_ERR;
    }

    return status;
}


/**************************************************************
*功  能：写数据到M1卡一块
*函数名：PcdWrite
*参  数: addr[IN]：块地址  
         pData[IN]：写入的数据，16字节
*返回值: 成功返回MI_OK
**************************************************************/
char PcdWrite(uint8_t addr,uint8_t *pData)
{
    char   status;
    uint8_t   unLen;
    uint8_t   i,ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        status = MI_ERR;
    }

    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {
            ucComMF522Buf[i] = *(pData +i);
        }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {
            status = MI_ERR;
        }
    }

    return status;
}


/**************************************************************
*功  能：读取M1卡一块数据
*函数名：PcdRead
*参  数: addr[IN]：块地址 
         pData[OUT]：读出的数据，16字节
*返回值: 成功返回MI_OK
**************************************************************/
char PcdRead(uint8_t addr,uint8_t *pData)
{
    char   status;
    uint8_t   unLen;
    uint8_t   i,ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
        {
            *(pData +i) = ucComMF522Buf[i];
        }
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}


/**************************************************************
*功  能：命令卡片进入休眠状态
*函数名：PcdHalt
*参  数: 无
*返回值: status,成功返回MI_OK
**************************************************************/
char PcdHalt(void)
{
    char status;

    uint8_t unLen;
    uint8_t ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;
}


/**************************************************************
*功  能：显示卡的卡号，以十六进制显示
*函数名：ShowID
*参  数: p
*返回值: 无
**************************************************************/
void ShowID(uint8_t *p)	 
{
    uint8_t num[9];
    uint8_t i;

    for(i=0; i<4; i++)
    {
        num[i*2] = p[i] / 16;
        num[i*2] > 9 ? (num[i*2] += '7') : (num[i*2] += '0');
        num[i*2+1] = p[i] % 16;
        num[i*2+1] > 9 ? (num[i*2+1] += '7') : (num[i*2+1] += '0');
    }
    num[8] = 0;
    sprintf(LCD_Show,"ID>>>%s \r\n", num);
    USART1_Send_String(LCD_Show);

}

/**************************************************************
*功  能：测试
*函数名：RC522_Test
*参  数: 无
*返回值: 无
**************************************************************/
void RC522_Test(void)
{
	uint8_t i = 0;
    STATUS = PcdRequest(PICC_REQALL,CT);//寻卡

    if(STATUS==MI_OK)//寻卡成功
    {
        STATUS=MI_ERR;
        STATUS = PcdAnticoll(SN);// 防冲撞 获得UID 存入SN
    }
    if (STATUS==MI_OK)//防冲撞成功
    {
        STATUS = MI_ERR;
        ShowID(SN); // 串口打印卡的ID号 UID
        STATUS = PcdSelect(SN);
    }
    else
    {
//        ILI9341_DispString_EN_CH(10,10,"卡片错误");
    }
/********************    选卡   **************************/	 
    if(STATUS == MI_OK)//选卡成功
    {
        STATUS = MI_ERR;
		// 验证A密钥 块地址 密码 SN
        STATUS = PcdAuthState(0x60, 0x09, KEY_A, SN);
		if(STATUS == MI_OK)//验证成功
		{
			sprintf(LCD_Show,"识别成功 \r\n");
            USART1_Send_String(LCD_Show);
		}
		else
		{
			sprintf(LCD_Show,"识别失败 \r\n");
            USART1_Send_String(LCD_Show);
		}
    }
/********************    读卡   **************************/	 
    if(STATUS == MI_OK)//验证成功
    {
        STATUS = MI_ERR;
		// 读取M1卡一块数据 块地址 读取的数据
        STATUS = PcdRead(Block_Addr, DATA);
		if(STATUS == MI_OK)//读卡成功
		{

			sprintf(LCD_Show,"卡片内容:");
            USART1_Send_String(LCD_Show);
			sprintf(LCD_read, "");
			for(i = 0; i < 16; i++)
			{
				sprintf(LCD_Show,"%2c", DATA[i]);
				sprintf(LCD_read,"%s%2c",LCD_read, DATA[i]);
                USART1_Send_String(LCD_Show);
			}
            USART1_Send_String(" \n ");
		}
		else
		{
			sprintf(LCD_Show,"读卡失败 \r\n");
            USART1_Send_String(LCD_Show);
		}
    }
/********************    写卡   **************************/	
	if(STATUS == MI_OK)//验证成功
	{
		STATUS = MI_ERR;
		// 写数据到M1卡一块
		STATUS = PcdWrite(Block_Addr, DATA1);
		if(STATUS == MI_OK)//写卡成功
		{
			sprintf(LCD_Show,"写卡成功 \r\n");
            USART1_Send_String(LCD_Show);
		}
		else
		{
			sprintf(LCD_Show,"写卡失败 \r\n");
            USART1_Send_String(LCD_Show);
		}
	}
	
	if(STATUS == MI_OK)//验证成功
    {
        STATUS = MI_ERR;
		// 读取M1卡一块数据 块地址 读取的数据
        STATUS = PcdRead(Block_Addr, DATA);
		if(STATUS == MI_OK)//读卡成功
		{
			// printf("DATA:%s\r\n", DATA);
			sprintf(LCD_Show,"写卡内容:");
            USART1_Send_String(LCD_Show);
			sprintf(LCD_wirte, "");
			for(i = 0; i < 16; i++)
			{
				sprintf(LCD_Show,"%2c", DATA[i]);
				sprintf(LCD_wirte,"%s%2c",LCD_wirte, DATA[i]);
                USART1_Send_String(LCD_Show);
			}
			sprintf(LCD_Show,"\n 显示写卡内容成功 \r\n");
            USART1_Send_String(LCD_Show);
            USART1_Send_String("\n");
			LCD_flag = 1;
		}
		else
		{
			sprintf(LCD_Show,"\n显示写卡内容失败 \r\n");
            USART1_Send_String(LCD_Show);
		}
    }
		
    if(STATUS == MI_OK)//验证成功
    {
        STATUS = MI_ERR;
        Delay_ms(100);
    }
}

