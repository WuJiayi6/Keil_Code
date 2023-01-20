#ifndef _SPI2_H_
#define _SPI2_H_
#include "sys.h"
 
uint8_t SPI_WriteByte(uint8_t Byte);
void SPI2_Hardware_Init(void);
void SPI_SetSpeed(uint8_t SpeedSet);

#endif
