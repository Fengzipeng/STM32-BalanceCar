#ifndef _iic_oled_H_
#define _iic_oled_H_
#include "stm32f10x.h"
#include "systick.h"
#include "public.h"
/***************************************/
#define SDA_Status  GPIO_ReadInputDataBit(OLED_GPIO_IIC,SDA) 
#define SDA_H GPIO_SetBits(OLED_GPIO_IIC,SDA)
#define SDA_L GPIO_ResetBits(OLED_GPIO_IIC,SDA)
#define SCL_H GPIO_SetBits(OLED_GPIO_IIC,SCL)
#define SCL_L GPIO_ResetBits(OLED_GPIO_IIC,SCL)
void IIC_oled_Init(void);
void IIC_oled_Start(void);
void IIC_oled_Stop(void);
void IIC_oled_Ack(void);
void IIC_oled_Nack(void);
int  IIC_oled_Wait_Ack(void);
void IIC_oled_Send_Byte(u8 txd);
u8 IIC_oled_Read_Byte(u8 ack);
#endif

