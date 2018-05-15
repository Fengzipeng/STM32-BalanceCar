#include "key.h"
void Key_Init()
{
	 GPIO_InitTypeDef  GPIO_InitStruct; 
	 SystemInit();  
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOF,ENABLE);
	 
	 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;//����ģʽ
	 GPIO_Init(GPIOA,&GPIO_InitStruct);  

   GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;
	 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	 GPIO_ResetBits(GPIOA,GPIO_Pin_0);//���һ���͵�ƽ
	 GPIO_ResetBits(GPIOF,GPIO_Pin_11);
}
//����ɨ�躯��
//����ֵ ��
//����ֵ 0��ɨ�費����������
//1 ��KEY1����
//2�� KEY2����
u8 KEY_PRESS_SCAN() 
{
	u8 press=0;
	if(KEY1_STATUS==KEY_PRESSED)
	{
		Delay_us(2000);
		if(KEY1_STATUS==KEY_PRESSED)
		{
			press=1;
		}
		while(press==1&&KEY1_STATUS==(!KEY_PRESSED));
		return 1;
	}
	if(KEY2_STATUS==KEY_PRESSED)
	{
		Delay_us(2000);
		if(KEY2_STATUS==KEY_PRESSED)
		{
			press=1;
		}
		while(press==1&&KEY2_STATUS==(!KEY_PRESSED));
		return 2;
	}
	return 0;
}


