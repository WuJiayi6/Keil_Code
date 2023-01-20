#include "RC522.h"

//////////////////////////////////////////////////////////
// M1����Ϊ16��������ÿ���������ĸ��飨��0����1����2����3�����
// ��16��������64���鰴���Ե�ַ���Ϊ��0~63
// ��0�������Ŀ�0�������Ե�ַ0�飩�����ڴ�ų��̴��룬�Ѿ��̻����ɸ���
// ÿ�������Ŀ�0����1����2Ϊ���ݿ飬�����ڴ������
// ÿ�������Ŀ�3Ϊ���ƿ飨���Ե�ַΪ:��3����7����11.....����������A����ȡ���ơ�����B��

/*******************************
*����˵����
*1--SDA  <----->PA4
*2--SCK  <----->PA5
*3--MOSI <----->PA7
*4--MISO <----->PA6
*5--����
*6--GND <----->GND
*7--RST <----->PB0
*8--VCC <----->VCC
************************************/
/*ȫ�ֱ���*/
unsigned char CT[2];//������
unsigned char SN[4]; //����
unsigned char DATA[16];//������� 
uint8_t KEY_A[6]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//��Կ
unsigned char DATA1[16]= {'W','J','Y','2','0','7','1','0','2','2','5'};
unsigned char STATUS;
unsigned char Block_Addr=0x08;// 0x08 ����2����0���飨����9�飩
char LCD_Show[50] = {0};
char LCD_read[50] = {0};
char LCD_wirte[50] = {0};
uint8_t LCD_flag = 0;


/*******************************************************************
��  �ܣ� RC522��ʼ��
�������� RC522_Init
��  ���� ��
����ֵ�� ��
********************************************************************/
void RC522_Init(void)
{
    SPI1_Hardware_Init();//SPI1��ʼ��
	RC522_Reset_Disable();	
	RC522_CS_Disable();    
    PcdReset();//RC522��λ
    M500PcdConfigISOType('A');//���ù�����ʽ

}


/*******************************************************************
��  �ܣ� ��RC522�Ĵ���
�������� ReadRawRC
��  ���� ucAddress,�Ĵ�����ַ
����ֵ�� ucReturn,�Ĵ����ĵ�ǰֵ
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
*��  �ܣ�дRC522�Ĵ���
*��������WriteRawRC
*��  ��: ucAddress,�Ĵ�����ַ  ucValue,д��Ĵ�����ֵ
*����ֵ: ��
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
*��  �ܣ���RC522�Ĵ�����λ
*��������SetBitMask
*��  ��: reg[IN]:�Ĵ�����ַ  mask[IN]:��λֵ
*����ֵ: ��
**************************************************************/
void SetBitMask(uint8_t reg,uint8_t mask)
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}


/**************************************************************
*��  �ܣ���RC522�Ĵ�����λ
*��������ClearBitMask
*��  ��: reg[IN]:�Ĵ�����ַ  mask[IN]:��λֵ
*����ֵ: ��
**************************************************************/
void ClearBitMask(uint8_t reg,uint8_t mask)
{
    uint8_t ucTemp;
    ucTemp = ReadRawRC(reg);
    WriteRawRC(reg, ucTemp & (~ mask));// clear bit mask
}


/**************************************************************
*��  �ܣ��������ߣ����߿�����رռ������1ms��
*��������PcdAntennaOn
*��  ��: ��
*����ֵ: ��
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
*��  �ܣ��ر�����
*��������PcdAntennaOff
*��  ��: ��
*����ֵ: ��
**************************************************************/
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg,0x03);
}


/*******************************************************************
��  �ܣ���λRC522 
��������PcdReset
��  ��: ��
����ֵ: �ɹ�����MI_OK
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
    WriteRawRC(ModeReg,0x3D);//��Mifare��ͨѶ��CRC��ʼֵ0x6363
    WriteRawRC(TReloadRegL,30);//16λ��ʱ����λ
    WriteRawRC(TReloadRegH,0);//16λ��ʱ����λ
    WriteRawRC(TModeReg, 0x8D);//�����ڲ���ʱ��������
    WriteRawRC(TPrescalerReg, 0x3E );//���ö�ʱ����Ƶϵ��
    WriteRawRC(TxAutoReg, 0x40 );//���Ʒ����ź�Ϊ100%ASK
    
}

/*******************************************************************
* ��  �ܣ�����RC522�Ĺ�����ʽ 
* ��������M500PcdConfigISOType
* ��  ��: ucType,������ʽ
* ����ֵ: ��
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
        PcdAntennaOn();//������
    }
}


/**************************************************************
*��  �ܣ�ͨ��RC522��ISO14443��ͨѶ
*��������PcdComMF522
*��  ��: Command[IN]:RC522������
*		 pInData[IN]:ͨ��RC522���͵���Ƭ������
*		 InLenByte[IN]:�������ݵ��ֽڳ���
*		 pOutData[OUT]:���յ��Ŀ�Ƭ��������
*		 *pOutLenBit[OUT]:�������ݵ�λ����
*����ֵ: ��
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
    case PCD_AUTHENT:       //Mifare��֤
        irqEn   = 0x12;    	//��������ж�����ErrIEn  ��������ж�IdleIEn
        waitFor = 0x10;    	//��֤Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ
        break;             
    case PCD_TRANSCEIVE:    //���շ��� ���ͽ���
        irqEn   = 0x77;    	//����TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
        waitFor = 0x30;    	//Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ�� �����жϱ�־λ
        break;             	
    default:
        break;
    }
    WriteRawRC(ComIEnReg,irqEn|0x80); //IRqInv��λ�ܽ�IRQ��Status1Reg��IRqλ��ֵ�෴
    ClearBitMask(ComIrqReg,0x80);	  //Set1��λ����ʱ��CommIRqReg������λ����
    WriteRawRC(CommandReg,PCD_IDLE);  //д��������
    SetBitMask(FIFOLevelReg,0x80);    //��λFlushBuffer����ڲ�FIFO�Ķ���дָ���Լ�ErrReg��BufferOvfl��־λ�����

    for (i=0; i<InLenByte; i++)
    {
        WriteRawRC(FIFODataReg, pIn [i]);    //д���ݽ�FIFOdata
    }
    WriteRawRC(CommandReg, Command);//д����
    if (Command == PCD_TRANSCEIVE)
    {
        SetBitMask(BitFramingReg,0x80);//StartSend��λ�������ݷ��� ��λ���շ�����ʹ��ʱ����Ч
    }
    i = 1000;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
    
    do                 //��֤��Ѱ���ȴ�ʱ��
    {
        n = ReadRawRC(ComIrqReg);    //��ѯ�¼��ж�
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));//�˳�����i=0,��ʱ���ж���д��������
    ClearBitMask(BitFramingReg,0x80);//��������StartSendλ

    if (i!=0)
    {
        if(!(ReadRawRC(ErrorReg)&0x1B))//�������־�Ĵ���BufferOfI CollErr ParityErr ProtocolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)  //�Ƿ�����ʱ���ж�
            {
                status = MI_NOTAGERR;
            }
            if (Command == PCD_TRANSCEIVE)
            {
                n = ReadRawRC(FIFOLevelReg);            //��FIFO�б�����ֽ���
                lastBits = ReadRawRC(ControlReg) & 0x07;//�����յ����ֽڵ���Чλ��
                if (lastBits) 
                {   
                    //N���ֽ�����ȥ1�����һ���ֽڣ�+���һλ��λ�� ��ȡ����������λ��                                       
                    *pOutLenBit = (n-1)*8 + lastBits;
                }                                       	
                else                                    
                {                                       
                    *pOutLenBit = n*8; //�����յ����ֽ������ֽ���Ч
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
*��  �ܣ�Ѱ��
*��  ��: req_code[IN]:Ѱ����ʽ
*		 0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
*		 0x26 = Ѱδ��������״̬�Ŀ�
*		 pTagType[OUT]����Ƭ���ʹ���
*		 0x4400 = Mifare_UltraLight
*		 0x0400 = Mifare_One(S50)
*		 0x0200 = Mifare_One(S70)
*		 0x0800 = Mifare_Pro(X)
*		 0x4403 = Mifare_DESFire
*����ֵ: �ɹ�����MI_OK
**************************************************************/
char PcdRequest(uint8_t req_code,uint8_t *pTagType)
{
    char   status;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg,0x08);//����ָʾMIFARECyptol��Ԫ��ͨ�Լ����п�������ͨ�ű����ܵ����
    WriteRawRC(BitFramingReg,0x07);//���͵����һ���ֽڵ���λ
    SetBitMask(TxControlReg,0x03);//TX1,TX2�ܽŵ�����źŴ��ݾ����͵��Ƶ�13.56�������ز��ź�
    
    ucComMF522Buf[0] = req_code;//���뿨Ƭ������
    
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x10))//Ѱ���ɹ����ؿ�����
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
*��  �ܣ�����ײ
*��������PcdAnticoll
*��  ��: pSnr����Ƭ���кţ�4�ֽ�
*����ֵ: �ɹ�����MI_OK
**************************************************************/
char PcdAnticoll(uint8_t *pSnr)
{
    char   status;
    uint8_t   i,snr_check=0;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg,0x08);//��MFCryptol Onλ ֻ�гɹ�ִ��MFAuthent����󣬸�λ������λ
    WriteRawRC(BitFramingReg,0x00);//����Ĵ��� ֹͣ�շ�
    ClearBitMask(CollReg,0x80);//��ValuesAfterColl���н��յ�λ�ڳ�ͻ�����

/*---------------------------------------------------------------- 
    �ο�ISO14443Э�飺
    https://blog.csdn.net/wowocpp/article/details/79910800
    PCD ���� SEL = ��93����NVB = ��20�������ֽ� 
    ��ʹ���е��ڳ���PICC����������UID CLn��ΪӦ��
------------------------------------------------------------------*/
    ucComMF522Buf[0] = 0x93;//��Ƭ����ͻ����
    ucComMF522Buf[1] = 0x20;

	 //���Ͳ��������� ���յ����ݴ洢��ucComMF522Buf
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

/** �뿨Ƭͨ�� **/
    if (status == MI_OK)//ͨ�ųɹ�
    {
		 //�յ���UID ����pSnr
        for (i=0; i<4; i++)
        {
            *(pSnr+i)  = ucComMF522Buf[i];//����UID
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
*��  �ܣ���RC522����CRC16����
*��������CalulateCRC
*��  ��: pIn,����CRC16������   
         len,����CRC16�������ֽڳ���
         pOut,��ż�������ŵ��׵�ַ
*����ֵ: ��
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
*��  �ܣ�ѡ����Ƭ
*��������PcdSelect
*��  ��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
*����ֵ: �ɹ�����MI_OK
**************************************************************/
char PcdSelect(uint8_t *pSnr)
{
    char   status;
    uint8_t   i;
    uint8_t   unLen;
    uint8_t   ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_ANTICOLL1;//����ײ 0x93
    
/* ����û�г�ͻ��PCD ָ��NVBΪ0x70��
 * ��ֵ��ʾPCD������������UID CLn��
 * ��40λUID CLn ƥ���PICC����SAK��ΪӦ��*/
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)//3 4 5 6λ���UID����7λһֱ���
    {
        ucComMF522Buf[i+2] = *(pSnr+i);
        ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);//CRC(ѭ������У��)
    ClearBitMask(Status2Reg,0x08);
    /*���Ͳ���������*/
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
*��  �ܣ���֤��Ƭ����
*��������PcdAuthState
*��  ��: auth_mode[IN]: 
         ������֤ģʽ 0x60 = ��֤A��Կ 0x61 = ��֤B��Կ
		 addr[IN]�����ַ   
         pKey[IN]������  
         pSnr[IN]����Ƭ���кţ�4�ֽ�
*����ֵ: �ɹ�����MI_OK
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
/* C �⺯�� 
   void *memcpy(void *str1, const void *str2, size_t n) 
   �Ӵ洢�� str2 ���� n ���ֽڵ��洢�� str1 */
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
*��  �ܣ�д���ݵ�M1��һ��
*��������PcdWrite
*��  ��: addr[IN]�����ַ  
         pData[IN]��д������ݣ�16�ֽ�
*����ֵ: �ɹ�����MI_OK
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
*��  �ܣ���ȡM1��һ������
*��������PcdRead
*��  ��: addr[IN]�����ַ 
         pData[OUT]�����������ݣ�16�ֽ�
*����ֵ: �ɹ�����MI_OK
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
*��  �ܣ����Ƭ��������״̬
*��������PcdHalt
*��  ��: ��
*����ֵ: status,�ɹ�����MI_OK
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
*��  �ܣ���ʾ���Ŀ��ţ���ʮ��������ʾ
*��������ShowID
*��  ��: p
*����ֵ: ��
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
*��  �ܣ�����
*��������RC522_Test
*��  ��: ��
*����ֵ: ��
**************************************************************/
void RC522_Test(void)
{
	uint8_t i = 0;
    STATUS = PcdRequest(PICC_REQALL,CT);//Ѱ��

    if(STATUS==MI_OK)//Ѱ���ɹ�
    {
        STATUS=MI_ERR;
        STATUS = PcdAnticoll(SN);// ����ײ ���UID ����SN
    }
    if (STATUS==MI_OK)//����ײ�ɹ�
    {
        STATUS = MI_ERR;
        ShowID(SN); // ���ڴ�ӡ����ID�� UID
        STATUS = PcdSelect(SN);
    }
    else
    {
//        ILI9341_DispString_EN_CH(10,10,"��Ƭ����");
    }
/********************    ѡ��   **************************/	 
    if(STATUS == MI_OK)//ѡ���ɹ�
    {
        STATUS = MI_ERR;
		// ��֤A��Կ ���ַ ���� SN
        STATUS = PcdAuthState(0x60, 0x09, KEY_A, SN);
		if(STATUS == MI_OK)//��֤�ɹ�
		{
			sprintf(LCD_Show,"ʶ��ɹ� \r\n");
            USART1_Send_String(LCD_Show);
		}
		else
		{
			sprintf(LCD_Show,"ʶ��ʧ�� \r\n");
            USART1_Send_String(LCD_Show);
		}
    }
/********************    ����   **************************/	 
    if(STATUS == MI_OK)//��֤�ɹ�
    {
        STATUS = MI_ERR;
		// ��ȡM1��һ������ ���ַ ��ȡ������
        STATUS = PcdRead(Block_Addr, DATA);
		if(STATUS == MI_OK)//�����ɹ�
		{

			sprintf(LCD_Show,"��Ƭ����:");
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
			sprintf(LCD_Show,"����ʧ�� \r\n");
            USART1_Send_String(LCD_Show);
		}
    }
/********************    д��   **************************/	
	if(STATUS == MI_OK)//��֤�ɹ�
	{
		STATUS = MI_ERR;
		// д���ݵ�M1��һ��
		STATUS = PcdWrite(Block_Addr, DATA1);
		if(STATUS == MI_OK)//д���ɹ�
		{
			sprintf(LCD_Show,"д���ɹ� \r\n");
            USART1_Send_String(LCD_Show);
		}
		else
		{
			sprintf(LCD_Show,"д��ʧ�� \r\n");
            USART1_Send_String(LCD_Show);
		}
	}
	
	if(STATUS == MI_OK)//��֤�ɹ�
    {
        STATUS = MI_ERR;
		// ��ȡM1��һ������ ���ַ ��ȡ������
        STATUS = PcdRead(Block_Addr, DATA);
		if(STATUS == MI_OK)//�����ɹ�
		{
			// printf("DATA:%s\r\n", DATA);
			sprintf(LCD_Show,"д������:");
            USART1_Send_String(LCD_Show);
			sprintf(LCD_wirte, "");
			for(i = 0; i < 16; i++)
			{
				sprintf(LCD_Show,"%2c", DATA[i]);
				sprintf(LCD_wirte,"%s%2c",LCD_wirte, DATA[i]);
                USART1_Send_String(LCD_Show);
			}
			sprintf(LCD_Show,"\n ��ʾд�����ݳɹ� \r\n");
            USART1_Send_String(LCD_Show);
            USART1_Send_String("\n");
			LCD_flag = 1;
		}
		else
		{
			sprintf(LCD_Show,"\n��ʾд������ʧ�� \r\n");
            USART1_Send_String(LCD_Show);
		}
    }
		
    if(STATUS == MI_OK)//��֤�ɹ�
    {
        STATUS = MI_ERR;
        Delay_ms(100);
    }
}

