#include "usart2.h"
void Usart2_Init()   //���ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
//	NVIC_InitTypeDef  NVIC_InitStructure;//�ж����ȼ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�򿪸��ù���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//����ʱ��
	/*********************�ܽ�����***************************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//txd
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//rxd
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*******************************************************/
	/***************��������******************/
	USART_InitStructure.USART_BaudRate=9600;//������9600
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;//У��λ--��
	USART_InitStructure.USART_HardwareFlowControl = //Ӳ����ʧ��
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;//ģʽ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	USART_Cmd(USART2,ENABLE); //����ʹ��
	/*************************************/
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�����ж�ʹ��
	USART_ClearFlag(USART2,USART_FLAG_TC);//��շ��ͱ�־λ
}
