#ifndef _ds18b20_H
#define _ds18b20_H

#include "stm32f10x.h"
#include "systick.h"
#define DQ GPIO_Pin_11
#define DS_GPIO GPIOG
#define DS18B20_DQ_H  GPIO_SetBits(DS_GPIO,DQ)
#define DS18B20_DQ_L  GPIO_ResetBits(DS_GPIO,DQ)
void DS18B20_Init(void);
void DQ_In(void);
void DQ_Out(void);
u8 DS18B20_ReadData(void);
void DS18B20_WriteData(u8 dat);
double Read_Temp(void);



#endif
