#ifndef _systick_H
#define _systick_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_ms(u32 x);
void Delay_us(u32 x);



#endif
