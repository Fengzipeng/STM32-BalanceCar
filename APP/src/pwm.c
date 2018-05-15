#include "pwm.h"
void PWM_Init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period=1999;//ARR
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_Prescaler=719;//分频系数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM3,ENABLE);//使能外设
	
	/*************PWM配置***************/
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//输出模式
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//使能
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	/***********************************/
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
}
void Direction_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
void Control(u8 IN0,u8 IN1,u8 STYB)
{
	if(IN0==1)
	{
	 GPIO_SetBits(GPIOC,GPIO_Pin_14);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_14);
	}
	if(IN1==1)
  {
	 GPIO_SetBits(GPIOC,GPIO_Pin_15);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_15);
	}
	if(STYB==1)
	{
	 GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
}

