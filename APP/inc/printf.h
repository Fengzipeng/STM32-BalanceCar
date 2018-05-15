#ifndef _printf_H_
#define _printf_H_
#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "public.h"
void Printf_Init(void);
int fputc(int ch, FILE *p);



#endif
