#ifndef _public_H
#define _public_H
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "led.h"
#include "systick.h"
#include "printf.h"
#include "at24cxx.h"
#include "key.h"
#include "flash.h"
#include "ili9341.h"
#include "lcd_driver.h"
#define ERR_NONE 0
#define ERR_TRUE 1
void Public(void);
extern u8 NRF_Received_Data[32];
#endif

