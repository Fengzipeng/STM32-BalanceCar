#include "iic_oled.h"
/*��Ҫ�л�IIC���ӹܽţ�ֻ��Ҫ�޸�GPIO�ں�Pin*/
#define SDA GPIO_Pin_3
#define SCL GPIO_Pin_4  
#define OLED_GPIO_IIC GPIOB 
void IIC_oled_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�����������
	 GPIO_InitStructure.GPIO_Pin=SDA | SCL;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(OLED_GPIO_IIC,&GPIO_InitStructure);
	 SDA_H; //����״̬
	 SCL_H; //����״̬
}
void SDA_Out()  //SDA���
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�����������
	 GPIO_InitStructure.GPIO_Pin=SDA;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(OLED_GPIO_IIC,&GPIO_InitStructure);
}
void SDA_In() //SDA����
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
	 GPIO_InitStructure.GPIO_Pin=SDA;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(OLED_GPIO_IIC,&GPIO_InitStructure);
}
void IIC_oled_Start(void) //��ֹ�ź�
{
	SDA_Out();
	SDA_H;
	SCL_H;
	Delay_us(5);
	SDA_L;
	Delay_us(6);
	SCL_L;
}
void IIC_oled_Stop(void) //��ֹ�ź�
{
	SDA_Out();
	SCL_L;
	SDA_L;
	SCL_H;
	Delay_us(5);
  SDA_H;
	Delay_us(6);
}
void IIC_oled_Ack(void) //��������Ӧ���ź�
{
	SDA_Out();
	SDA_L;
	Delay_us(2);
	SCL_H;
  Delay_us(5);
  SCL_L;
}
void IIC_oled_Nack(void) //����������Ӧ���ź�
{
	SDA_Out();
	SDA_H;
	Delay_us(2);
	SCL_H;
  Delay_us(5);
  SCL_L;
}
int  IIC_oled_Wait_Ack(void)   //�����ȴ��ӻ�Ӧ��
{
	int temp=0;
	SDA_In(); //SDA����ģʽ
	SDA_H;
	Delay_us(1);
	SCL_H;
	Delay_us(1);
	while(SDA_Status)
	{
		temp++;
		if(temp>250) 
		{
			IIC_oled_Stop();   //1--Ӧ��ʧ��
			return 1;     //0--Ӧ��ɹ�
		}
	}
	SCL_L;
	return 0;
}
void IIC_oled_Send_Byte(u8 txd)
{
	u8 i=0;
	SDA_Out();
	SCL_L;
	for(i=0;i<8;i++)
	{
		if( (txd&0x80)>0)
		{
			SDA_H;
		}
		else SDA_L;
		txd<<=1;
		SCL_H;
		Delay_us(2);
		SCL_L;
		Delay_us(2);
	}
}
u8 IIC_oled_Read_Byte(u8 ack)  //1--Ӧ�� 0--��Ӧ��
{
	u8 i=0;
	u8 Receive=0;
	SDA_In();
	for(i=0;i<8;i++)
	{
		SCL_L;
		Delay_us(2);
		SCL_H;
    Receive<<=1;
		if(SDA_Status)
		{
			Receive++;
		}
		Delay_us(1);
	}
	if(ack==0)
	{
		IIC_oled_Nack();
	}
	else 
	{
		IIC_oled_Ack();
	}
	return Receive;
}






