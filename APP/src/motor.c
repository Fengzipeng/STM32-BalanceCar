/***********************************/
/*��������ļ�*/
//PWM���ת����������ƺ���
//���ת����������ƺ���
#include "motor.h"
void Motor_Control_PWM_Init()
{		 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// TIM1
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);//��ȫӳ��
  //���ø�����Ϊ�����������,���TIM1 CH1 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM1_CH1 //TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 7200-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��	 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	Direction_Init();
}
/********����������***********/
void Direction_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void Direction_Control_A(u8 AIN1,u8 AIN2)
{
	if(AIN1==1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}
	else GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
	if(AIN2==1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
	}
	else GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
void Direction_Control_B(u8 BIN1,u8 BIN2)
{
	if(BIN1==1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
	}
	else GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	if(BIN2==1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
	}
	else GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
void Direction_Control(u8 channel ,u8 Directiion)
{
	if(channel==1)
		{   if(Directiion==1) 
			{
			 GPIO_SetBits(GPIOB,GPIO_Pin_12);
	     GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			}
	    else
			{ 
				GPIO_SetBits(GPIOB,GPIO_Pin_13);
	      GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			}
		}
	else 
	{
			if(Directiion==1) 
			{
			 GPIO_SetBits(GPIOB,GPIO_Pin_14);
	     GPIO_ResetBits(GPIOB,GPIO_Pin_15);
			}
	    else
			{ 
				GPIO_SetBits(GPIOB,GPIO_Pin_15);
	      GPIO_ResetBits(GPIOB,GPIO_Pin_14);
			}
	}
}
