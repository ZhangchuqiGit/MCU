#ifndef BH1750_H
#define BH1750_H

#include "stm32f10x.h"

#define SlaveAddress 0x46
#define PWN_ON 0x01
#define Reset 0x07

void BH1750_Write(unsigned char REG_Address);
u16 BH1750_Read(void);
void BH1750_Init(void);

#endif

