#include <timer3.h> //��C�ļ����ö�ʱ��3������ÿʮ�������һ���ж�
void TIM3_Init()
{
	NVIC_InitTypeDef  NVIC_InitStructure;//�ж����ȼ�
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//�ҽ�ʱ��
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//��ʱ�����
	/**************���ö�ʱ��****************/
	TIM_TimeBaseInitStructure.TIM_Period=50;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;//��Ƶϵ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	/***************ʹ��*******************/
	TIM_Cmd(TIM3,ENABLE);//ʹ������
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  //ʹ���ж�
  /**************�������ȼ�****************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //����
  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn; //ͨ��ѡ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   //��Ӧ���ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ��
  NVIC_Init(&NVIC_InitStructure); //��ʼ��
}
