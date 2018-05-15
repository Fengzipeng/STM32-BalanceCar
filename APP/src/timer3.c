#include <timer3.h> //本C文件配置定时器3，用于每十毫秒产生一个中断
void TIM3_Init()
{
	NVIC_InitTypeDef  NVIC_InitStructure;//中断优先级
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//挂接时钟
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//定时器清除
	/**************配置定时器****************/
	TIM_TimeBaseInitStructure.TIM_Period=50;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;//分频系数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	/***************使能*******************/
	TIM_Cmd(TIM3,ENABLE);//使能外设
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  //使能中断
  /**************设置优先级****************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //分组
  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn; //通道选择
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   //响应优先级 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能
  NVIC_Init(&NVIC_InitStructure); //初始化
}
