#include <usart.h>
/*
输入参数
Usart_Num：串口号
BaudRate：波特率
ifable_exit：是否使能中断
*/
u8 Usart_Init(u8 Usart_Num,u32 BaudRate,u8 ifable_exit)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
  NVIC_InitTypeDef  NVIC_InitStructure;//中断优先级
	if(Usart_Num>3) 
		   return 0;//输入串口号错误，初始化失败
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开复用功能
	switch(Usart_Num)
	{
		case 1:
		{
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);break;//串口时钟
		}
		case 2:
		{
	    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);break;//串口时钟
		}
		case 3:
		{
	    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);break;//串口时钟
		}
	}
	/*********************管脚配置***************************/
	//*****************TXD****************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	if(Usart_Num==1) 
	{
		GPIO_InitStructure.GPIO_Pin=Usart1_GPIO_TXD;//txd
	}
	else if(Usart_Num==2)
	{
		GPIO_InitStructure.GPIO_Pin=Usart2_GPIO_TXD;//txd
	}
	else GPIO_InitStructure.GPIO_Pin=Usart3_GPIO_TXD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	if(Usart_Num==2||Usart_Num==1)
	    GPIO_Init(GPIOA,&GPIO_InitStructure);
	else GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*********************************************/
	/***********************RXD*****************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	if(Usart_Num==1) 
	{
		GPIO_InitStructure.GPIO_Pin=Usart1_GPIO_RXD;//txd
	}
	else if(Usart_Num==2)
	{
		GPIO_InitStructure.GPIO_Pin=Usart2_GPIO_RXD;//txd
	}
	else GPIO_InitStructure.GPIO_Pin=Usart3_GPIO_RXD;
	if(Usart_Num==2||Usart_Num==1)
	    GPIO_Init(GPIOA,&GPIO_InitStructure);
	else GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*****************中断配置****************/
	if(1==ifable_exit)
	{
		switch(Usart_Num)
		{                 
			case 1: NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;break;
			case 2: NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;break;
			case 3: NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;break;
		}
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//子优先级3
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	 NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	}
	/***************串口配置******************/
	USART_InitStructure.USART_BaudRate=BaudRate;//波特率9600
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位--无
	USART_InitStructure.USART_HardwareFlowControl = //硬件流失能
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;//模式
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	switch (Usart_Num)
	{
		case 1: 
			{
			   USART_Init(USART1,&USART_InitStructure);
	       USART_Cmd(USART1,ENABLE);break; //串口使能
			}
		case 2:
			{
			   USART_Init(USART2,&USART_InitStructure);
	       USART_Cmd(USART2,ENABLE);break; //串口使能
			}
		case 3:
			{
			   USART_Init(USART3,&USART_InitStructure);
	       USART_Cmd(USART3,ENABLE);break; //串口使能
			}
		}
	/*************************************/
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//接收中断使能
	 USART_ClearFlag(USART1,USART_FLAG_TC);//清空发送标志位
	return 1; //初始化成功
}
//串口中断处理函数
/*void USART3_IRQHandler(void)
{
 if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据 
      USART_SendData(USART1,DataScope_OutPut_Buffer[i]); //发送数据
		  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=1);//等待数据发送完毕
		  USART_ClearFlag(USART1,USART_FLAG_TC);//清空发送完成标志位
}
void USART2_IRQHandler(void);
void USART1_IRQHandler(void)*/
