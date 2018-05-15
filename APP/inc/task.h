#ifndef _task_H
#define _task_H
#include "stm32f10x.h"
#include "public.h"
#include "systick.h"
#include "ucos_ii.h"
#include "os_cfg.h"
#include "os_cpu.h"
#define STK_SIZE 64
extern OS_STK LED_TASK_STK[STK_SIZE];
extern OS_STK PRINTF_TASK_STK[STK_SIZE];
extern OS_STK MPU_TASK_STK[STK_SIZE];
extern OS_STK AT24Cxx_TASK_STK[STK_SIZE];
extern OS_STK KEY_TASK_STK[STK_SIZE];
extern OS_STK FLASH_TASK_STK[STK_SIZE];
extern OS_STK LCD_TASK_STK[STK_SIZE];
void KEY_Task(void *pdata);
void LED_Task(void *pdata);
void Print_Data(void *pdata);
void MPU_Task(void *pdata);
void AT24Cxx_Task(void *pdata);
void FLASH_Task(void *pdata);
void LCD_Task(void *pdata);
extern OS_EVENT *MPU_EVENT_Q; 
extern void *mpu_data[3];
#endif
