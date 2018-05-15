#ifndef _beep_H
#define _beep_H

#include <stm32f10x.h>
#define BEEP GPIO_Pin_5

void Beep_Init(void);
void Beep_Work(void);

#endif
