#include "led.h"
void Led_Init(void)  //LED��ʼ������
{
	 GPIO_InitTypeDef  GPIO_InitStruct; //�ṹ���������
	 SystemInit();  //��ϵͳʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO\
	 |RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOB\
	|RCC_APB2Periph_GPIOF,ENABLE);
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//����JTAG  ��������ʱ��
	 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;  //LED1
	 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//�������ģʽ
	 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOC,&GPIO_InitStruct); 
	
	 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;  //LED2
	 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//�������ģʽ
	 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOD,&GPIO_InitStruct);  
	
	 GPIO_SetBits(GPIOC,GPIO_Pin_0);
	 GPIO_SetBits(GPIOD,GPIO_Pin_3);
}
void LED_Flash(u8 times)
{
	u8 i;
	for(i=0;i<times;i++)
	 {
		 LED1_ON;
		 LED2_ON;
		 Delay_ms(200);
		 LED1_OFF;
		 LED2_OFF;
		 Delay_ms(200);
	 }
}

