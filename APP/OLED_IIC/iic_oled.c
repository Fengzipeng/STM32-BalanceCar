#include "iic_oled.h"
/*若要切换IIC连接管脚，只需要修改GPIO口和Pin*/
#define SDA GPIO_Pin_3
#define SCL GPIO_Pin_4  
#define OLED_GPIO_IIC GPIOB 
void IIC_oled_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//复用推挽输出
	 GPIO_InitStructure.GPIO_Pin=SDA | SCL;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(OLED_GPIO_IIC,&GPIO_InitStructure);
	 SDA_H; //空闲状态
	 SCL_H; //空闲状态
}
void SDA_Out()  //SDA输出
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//复用推挽输出
	 GPIO_InitStructure.GPIO_Pin=SDA;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(OLED_GPIO_IIC,&GPIO_InitStructure);
}
void SDA_In() //SDA输入
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	 GPIO_InitStructure.GPIO_Pin=SDA;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(OLED_GPIO_IIC,&GPIO_InitStructure);
}
void IIC_oled_Start(void) //起止信号
{
	SDA_Out();
	SDA_H;
	SCL_H;
	Delay_us(5);
	SDA_L;
	Delay_us(6);
	SCL_L;
}
void IIC_oled_Stop(void) //终止信号
{
	SDA_Out();
	SCL_L;
	SDA_L;
	SCL_H;
	Delay_us(5);
  SDA_H;
	Delay_us(6);
}
void IIC_oled_Ack(void) //主机发出应答信号
{
	SDA_Out();
	SDA_L;
	Delay_us(2);
	SCL_H;
  Delay_us(5);
  SCL_L;
}
void IIC_oled_Nack(void) //主机发出非应答信号
{
	SDA_Out();
	SDA_H;
	Delay_us(2);
	SCL_H;
  Delay_us(5);
  SCL_L;
}
int  IIC_oled_Wait_Ack(void)   //主机等待从机应答
{
	int temp=0;
	SDA_In(); //SDA输入模式
	SDA_H;
	Delay_us(1);
	SCL_H;
	Delay_us(1);
	while(SDA_Status)
	{
		temp++;
		if(temp>250) 
		{
			IIC_oled_Stop();   //1--应答失败
			return 1;     //0--应答成功
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
u8 IIC_oled_Read_Byte(u8 ack)  //1--应答 0--非应答
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






