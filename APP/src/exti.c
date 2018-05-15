#include "exti.h"
#include "stm32f10x.h"
void EXTI2_Init()
{
	EXTI_InitTypeDef  EXTI_InitStructure;//�ⲿ�жϽṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�ṹ��
	NVIC_InitTypeDef  NVIC_InitStructure;//�ж����ȼ�
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//�ҽӶ˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�������ù���
	/***********GPIO�ܽ�����************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*******ӳ��PE2�ܽŵ��ⲿ�ж�2����*******/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);
	/*************�����ж�**************/
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��س�����
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStructure);
	/**********�����ж����ȼ�************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel =EXTI2_IRQn; //ͨ��ѡ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   //��Ӧ���ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ��
  NVIC_Init(&NVIC_InitStructure); //��ʼ��
}
void EXTI5_Init()
{  
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIO�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	            //�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIO
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
}

