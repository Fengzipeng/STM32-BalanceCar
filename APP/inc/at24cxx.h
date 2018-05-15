#ifndef _at24cxx_H_
#define _at24cxx_H_
#include "stm32f10x.h"
#include "systick.h"
#define AT24C01  127
#define AT24C02  255
#define AT24C04  511
#define AT24C08  1023
#define AT24C16  2047
#define AT24C32  4095
#define AT24C64  8191
#define AT24C128 16383
#define AT24C256 32767
#define EE_TYPE  AT24C256

#define AT24Cxx_I2C_SCL GPIO_Pin_6	  //PB6
#define AT24Cxx_I2C_SDA GPIO_Pin_7	  //PB7
#define AT24Cxx_GPIO_I2C GPIOB

#define AT24Cxx_I2C_SCL_H GPIO_SetBits(AT24Cxx_GPIO_I2C,AT24Cxx_I2C_SCL)
#define AT24Cxx_I2C_SCL_L GPIO_ResetBits(AT24Cxx_GPIO_I2C,AT24Cxx_I2C_SCL)

#define AT24Cxx_I2C_SDA_H GPIO_SetBits(AT24Cxx_GPIO_I2C,AT24Cxx_I2C_SDA)
#define AT24Cxx_I2C_SDA_L GPIO_ResetBits(AT24Cxx_GPIO_I2C,AT24Cxx_I2C_SDA)

u8 AT24Cxx_ReadOneByte(u16 addr);
void AT24Cxx_WriteOneByte(u16 addr,u8 dat);
u16 AT24Cxx_ReadTwoByte(u16 addr);
void AT24Cxx_WriteTwoByte(u16 addr,u16 dat);
/* 声明全局函数 */
void AT24Cxx_INIT(void);
void AT24Cxx_I2C_SDA_OUT(void);
void AT24Cxx_I2C_SDA_IN(void);
void AT24Cxx_I2C_Start(void);
void AT24Cxx_I2C_Stop(void);
void AT24Cxx_I2C_Ack(void);
void AT24Cxx_I2C_NAck(void);
u8   AT24Cxx_I2C_Wait_Ack(void);
void AT24Cxx_I2C_Send_Byte(u8 txd);
u8   AT24Cxx_I2C_Read_Byte(u8 ack);
#endif
