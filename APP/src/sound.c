#include "sound.h"
float Distance=0;
void Sound_Init()
{
	  GPIO_InitTypeDef GPIO_InitStructure;    
	  TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	  NVIC_InitTypeDef  NVIC_InitStructure;//中断优先级

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);  //挂载时钟

    GPIO_InitStructure.GPIO_Pin = Sound_Trig;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(Sound_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Sound_Echo;                   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;         
    GPIO_Init(Sound_GPIO, &GPIO_InitStructure); 
    GPIO_ResetBits(Sound_GPIO, Sound_Echo);

	  TIM_TimeBaseInitStructure.TIM_Period = 0xffff; 
    TIM_TimeBaseInitStructure.TIM_Prescaler =72-1; 
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); 
		
	  TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);  //上升沿捕获
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //分组
    NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn; //通道选择 TIM5时钟中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   //响应优先级 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能
    NVIC_Init(&NVIC_InitStructure); //初始化
		
		TIM_Cmd(TIM3,ENABLE);
		TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,ENABLE);//使能更新中断和捕获中断
}
void SR04_Start()
{
	GPIO_SetBits(Sound_GPIO,Sound_Trig);
	Delay_us(15);
	GPIO_ResetBits(Sound_GPIO,Sound_Trig);
}
u16 TIM3CH1_CAPTURE_STA,TIM3CH1_CAPTURE_VAL;
u8 Distance_Flag=0;
u32 Time_Count;
void TIM3_IRQHandler()   //定时器 5 输入捕获中断函数 
{ 
   if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获 
    { 
      if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
      { 
          if(TIM3CH1_CAPTURE_STA&0X40) //已经捕获到高电平了 
          { 
              if((TIM3CH1_CAPTURE_STA&0x3f)==0x3f)//高电平太长了 
             {  
              TIM3CH1_CAPTURE_STA|=0x80; //标记成功捕获了一次 
						  TIM3CH1_CAPTURE_VAL=0xffff;   
             }
             else TIM3CH1_CAPTURE_STA++;   
          } 
      }  
    } 
 if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) //捕获 1 发生捕获事件 
 { 
    if( TIM3CH1_CAPTURE_STA&0X40 ) //捕获到一个下降沿 
   { 
     TIM3CH1_CAPTURE_STA|=0X80; //标记成功捕获到一次上升沿  
     TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5); //获得 TIMx 输入捕获 1 的值 
     TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //设置为上升沿捕获  
		 Distance_Flag=1;//捕获成功
   } 
  else 
   { 
    TIM3CH1_CAPTURE_STA=0; //清空  
    TIM3CH1_CAPTURE_VAL=0;  
    TIM_SetCounter(TIM3,0);  
    TIM3CH1_CAPTURE_STA|=0X40; //标记捕获到了上升沿  
    TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling); //设置为下降沿捕获 
   }  
  } 
  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位 
  if(Distance_Flag==1)
	{
		TIM3CH1_CAPTURE_STA=TIM3CH1_CAPTURE_STA&0x7f;
		Time_Count=TIM3CH1_CAPTURE_STA&0x3f*65536+TIM3CH1_CAPTURE_VAL;//us
		Distance=Time_Count/1000*340/2;//距离的单位是mm
		Time_Count=0;
		Distance_Flag=0;
	}
}

