#ifndef _pwm_H
#define _pwm_H
#include "stm32f10x.h"
#include "public.h"
void PWM_Init(void);
void Direction_Init(void);
void Control(u8 IN0,u8 IN1,u8 STYB);
#endif
