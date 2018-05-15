#ifndef _ADC_H
#define _ADC_H

#include "public.h"
#include "stm32f10x.h"

#define Battery_Ch 4
void ADC1_Init(void);
u16 Get_Adc(u8 ch);
float Get_Voltage(void);



#endif
