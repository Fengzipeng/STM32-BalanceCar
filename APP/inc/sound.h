#ifndef SOUND_H
#define SOUND_H
#include "public.h"
#include "stm32f10x.h"
#define Sound_Trig GPIO_Pin_0
#define Sound_Echo GPIO_Pin_1
#define Sound_GPIO GPIOC
extern float Distance;
void SR04_Start(void);
void Sound_Init(void);







#endif
