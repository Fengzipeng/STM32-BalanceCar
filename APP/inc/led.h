#ifndef _led_H
#define _led_H

#include "stm32f10x.h"
#include "public.h"
#include "systick.h"
#define LED1 1
#define LED2 2
#define LED1_Status GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0)
#define LED1_ON GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define LED1_OFF GPIO_SetBits(GPIOC,GPIO_Pin_0)

#define LED2_Status GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_3)
#define LED2_ON GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define LED2_OFF GPIO_SetBits(GPIOD,GPIO_Pin_3)


void LED_Flash(u8 times);
void Led_Init(void);
void LEDx_Flash(u8 LEDx,u8 times);
#endif
