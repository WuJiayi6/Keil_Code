#include "dma.h"

uint8_t TransferStatus;/* �����űȽϽ������ */

/* ����aSRC_Const_Buffer������ΪDMA��������Դ
 * const�ؼ��ֽ�aSRC_Const_Buffer�����������Ϊ��������
 * ��ʾ���ݴ洢���ڲ���FLASH�� */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
																		
/* ����DMA����Ŀ��洢��,�洢���ڲ���SRAM�� */
uint32_t aDST_Buffer[BUFFER_SIZE];

//DMA����									
void DMA_Hardware_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);// ����DMAʱ��
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;// Դ���ݵ�ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)aDST_Buffer;// Ŀ���ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//����:���赽�洢����������������ڲ���FLASH��
	DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;// �����С	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;// ���裨�ڲ���FLASH����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// �ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;// �������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;// �ڴ����ݵ�λ	 
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;// DMAģʽ��һ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // DMAģʽ��ѭ��ģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;// ���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;// ʹ���ڴ浽�ڴ�Ĵ���

	DMA_Init(DMA1_Channel6, &DMA_InitStructure);// ����DMAͨ��6
	DMA_ClearFlag(DMA1_FLAG_TC6);//���DMA������������ɱ�־λ
	DMA_Cmd(DMA1_Channel6,ENABLE);// ʹ��DMA
}									
	


/* �ж�ָ�����ȵ���������Դ�Ƿ���ȫ��ȣ�
�����ȫ��ȷ���1��ֻҪ����һ�����ݲ���ȷ���0 */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* ���ݳ��ȵݼ� */
  while(BufferLength--)
  {
    if(*pBuffer != *pBuffer1)/* �ж���������Դ�Ƿ��Ӧ��� */
    {
      return 0;/* ��Ӧ����Դ����������˳�������������0 */
    }
    /* ������������Դ�ĵ�ַָ�� */
    pBuffer++;
    pBuffer1++;
  }
  return 1; /* ����жϲ��Ҷ�Ӧ������� */  
}


