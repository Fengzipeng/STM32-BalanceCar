#ifndef _usart_H_
#define _usart_H_

#include "stm32f10x.h"
#include "public.h"
/*串口位置      
              TXD  RXD
      USART1  PA9  PA10
      USART2  PA2  PA3
      USART3  PB10 PB11
*/
u8 Usart_Init(u8 Usart_Num,u32 BaudRate,u8 ifable_exit);//串口初始化函数
#define Usart1_GPIO_TXD  GPIO_Pin_9 //PA9
#define Usart1_GPIO_RXD  GPIO_Pin_10 //PA10
#define Usart1_GPIO      GPIOA 

#define Usart2_GPIO_TXD  GPIO_Pin_2 //PA2
#define Usart2_GPIO_RXD  GPIO_Pin_3 //PA3
#define Usart2_GPIO      GPIOA 

#define Usart3_GPIO_TXD  GPIO_Pin_10 //PB10
#define Usart3_GPIO_RXD  GPIO_Pin_11 //PA11
#define Usart3_GPIO      GPIOB
#endif
