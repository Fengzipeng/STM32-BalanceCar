#include "usart3.h"
void Usart3_Init()   //串口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
  //NVIC_InitTypeDef  NVIC_InitStructure;//中断优先级
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开复用功能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//串口时钟
	/*********************管脚配置***************************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//txd
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//rxd
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*******************************************************/
	/***************串口配置******************/
	USART_InitStructure.USART_BaudRate=9600;//波特率9600
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位--无
	USART_InitStructure.USART_HardwareFlowControl = //硬件流失能
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;//模式
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&USART_InitStructure);
	USART_Cmd(USART3,ENABLE); //串口使能
	/*************************************/
	//USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//接收中断使能
	USART_ClearFlag(USART3,USART_FLAG_TC);//清空发送标志位
}
