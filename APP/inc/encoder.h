#ifndef _ENCODER_H
#define _ENCODER_H
#include "public.h"
#include "stm32f10x.h"
void Encoder_Init(void);
void TIM2_Encoder_Init(void);
void TIM4_Encoder_Init(void);
int Read_Encoder(u8 TIMx);
#endif

