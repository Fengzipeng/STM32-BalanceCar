#include <usart.h>
/*
�������
Usart_Num�����ں�
BaudRate��������
ifable_exit���Ƿ�ʹ���ж�
*/
u8 Usart_Init(u8 Usart_Num,u32 BaudRate,u8 ifable_exit)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
  NVIC_InitTypeDef  NVIC_InitStructure;//�ж����ȼ�
	if(Usart_Num>3) 
		   return 0;//���봮�ںŴ��󣬳�ʼ��ʧ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�򿪸��ù���
	switch(Usart_Num)
	{
		case 1:
		{
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);break;//����ʱ��
		}
		case 2:
		{
	    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);break;//����ʱ��
		}
		case 3:
		{
	    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);break;//����ʱ��
		}
	}
	/*********************�ܽ�����***************************/
	//*****************TXD****************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
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
	/*****************�ж�����****************/
	if(1==ifable_exit)
	{
		switch(Usart_Num)
		{                 
			case 1: NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;break;
			case 2: NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;break;
			case 3: NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;break;
		}
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//�����ȼ�3
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	 NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	}
	/***************��������******************/
	USART_InitStructure.USART_BaudRate=BaudRate;//������9600
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;//У��λ--��
	USART_InitStructure.USART_HardwareFlowControl = //Ӳ����ʧ��
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;//ģʽ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	switch (Usart_Num)
	{
		case 1: 
			{
			   USART_Init(USART1,&USART_InitStructure);
	       USART_Cmd(USART1,ENABLE);break; //����ʹ��
			}
		case 2:
			{
			   USART_Init(USART2,&USART_InitStructure);
	       USART_Cmd(USART2,ENABLE);break; //����ʹ��
			}
		case 3:
			{
			   USART_Init(USART3,&USART_InitStructure);
	       USART_Cmd(USART3,ENABLE);break; //����ʹ��
			}
		}
	/*************************************/
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�����ж�ʹ��
	 USART_ClearFlag(USART1,USART_FLAG_TC);//��շ��ͱ�־λ
	return 1; //��ʼ���ɹ�
}
//�����жϴ�����
/*void USART3_IRQHandler(void)
{
 if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ����� 
      USART_SendData(USART1,DataScope_OutPut_Buffer[i]); //��������
		  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=1);//�ȴ����ݷ������
		  USART_ClearFlag(USART1,USART_FLAG_TC);//��շ�����ɱ�־λ
}
void USART2_IRQHandler(void);
void USART1_IRQHandler(void)*/
