#ifndef _motor_H_
#define _motor_H_

#include "stm32f10x.h"
#include "public.h"
#define PWMA   TIM1->CCR1 //PA8
#define PWMB   TIM1->CCR4  //PA11
void Motor_Control_PWM_Init(void);
void Direction_Init(void);
void Direction_Control_A(u8 AIN1,u8 AIN2);
void Direction_Control_B(u8 BIN1,u8 BIN2);
void Direction_Control(u8 channel ,u8 Direction);
#endif
