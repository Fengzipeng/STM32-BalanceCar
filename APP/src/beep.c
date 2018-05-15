#include <stm32f10x.h>
#include <beep.h>
void delay(long x)
{
	while(x--);
}
void Beep_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=BEEP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void Beep_Work()
{     
		 //GPIO_SetBits(GPIOB,BEEP);
	   //GPIO_ResetBits(GPIOB,BEEP);
	   GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);
	   delay(1000);
	   GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);
	   delay(1000);
}
