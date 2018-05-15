#include "sound.h"
float Distance=0;
void Sound_Init()
{
	  GPIO_InitTypeDef GPIO_InitStructure;    
	  TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	  NVIC_InitTypeDef  NVIC_InitStructure;//�ж����ȼ�

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);  //����ʱ��

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
		
	  TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);  //�����ز���
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //����
    NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn; //ͨ��ѡ�� TIM5ʱ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   //��Ӧ���ȼ� 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ��
    NVIC_Init(&NVIC_InitStructure); //��ʼ��
		
		TIM_Cmd(TIM3,ENABLE);
		TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,ENABLE);//ʹ�ܸ����жϺͲ����ж�
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
void TIM3_IRQHandler()   //��ʱ�� 5 ���벶���жϺ��� 
{ 
   if((TIM3CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ����� 
    { 
      if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
      { 
          if(TIM3CH1_CAPTURE_STA&0X40) //�Ѿ����񵽸ߵ�ƽ�� 
          { 
              if((TIM3CH1_CAPTURE_STA&0x3f)==0x3f)//�ߵ�ƽ̫���� 
             {  
              TIM3CH1_CAPTURE_STA|=0x80; //��ǳɹ�������һ�� 
						  TIM3CH1_CAPTURE_VAL=0xffff;   
             }
             else TIM3CH1_CAPTURE_STA++;   
          } 
      }  
    } 
 if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) //���� 1 ���������¼� 
 { 
    if( TIM3CH1_CAPTURE_STA&0X40 ) //����һ���½��� 
   { 
     TIM3CH1_CAPTURE_STA|=0X80; //��ǳɹ�����һ��������  
     TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5); //��� TIMx ���벶�� 1 ��ֵ 
     TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //����Ϊ�����ز���  
		 Distance_Flag=1;//����ɹ�
   } 
  else 
   { 
    TIM3CH1_CAPTURE_STA=0; //���  
    TIM3CH1_CAPTURE_VAL=0;  
    TIM_SetCounter(TIM3,0);  
    TIM3CH1_CAPTURE_STA|=0X40; //��ǲ�����������  
    TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling); //����Ϊ�½��ز��� 
   }  
  } 
  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ 
  if(Distance_Flag==1)
	{
		TIM3CH1_CAPTURE_STA=TIM3CH1_CAPTURE_STA&0x7f;
		Time_Count=TIM3CH1_CAPTURE_STA&0x3f*65536+TIM3CH1_CAPTURE_VAL;//us
		Distance=Time_Count/1000*340/2;//����ĵ�λ��mm
		Time_Count=0;
		Distance_Flag=0;
	}
}

