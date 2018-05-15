#include "at24cxx.h"
u8 AT24Cxx_ReadOneByte(u16 addr)//读取一个字节
{                         //add--数据地址
	u8 temp;
	AT24Cxx_I2C_Start();
  AT24Cxx_I2C_Send_Byte(0xa0);
	AT24Cxx_I2C_Wait_Ack();             //等待应答
	AT24Cxx_I2C_Send_Byte(addr>>8);
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(addr%256);
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(0xa1);
	AT24Cxx_I2C_Wait_Ack();
	temp=AT24Cxx_I2C_Read_Byte(0);//非应答
	AT24Cxx_I2C_Stop();
	return temp;
}
void AT24Cxx_WriteOneByte(u16 addr,u8 dat)
{
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(0xa0);
	AT24Cxx_I2C_Wait_Ack();             //等待应答
	AT24Cxx_I2C_Send_Byte(addr>>8);
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(addr%256);
	AT24Cxx_I2C_Wait_Ack();
	//AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(dat);
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Stop();
	Delay_us(10000);
}
u16 AT24Cxx_ReadTwoByte(u16 addr)//读两字节
{
	u16 temp;
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(0xa0);
	AT24Cxx_I2C_Wait_Ack();             //等待应答
	AT24Cxx_I2C_Send_Byte(addr>>8);
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(addr%256);
	AT24Cxx_I2C_Wait_Ack();
	
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(0xa1);
	AT24Cxx_I2C_Wait_Ack();
	temp=AT24Cxx_I2C_Read_Byte(1);//非应答
	temp<<=8;
	temp|=AT24Cxx_I2C_Read_Byte(0);
	AT24Cxx_I2C_Stop();
	return temp;
}
void AT24Cxx_WriteTwoByte(u16 addr,u16 dat)
{
	AT24Cxx_I2C_Start();
  if(EE_TYPE>AT24C16) 
	{
		AT24Cxx_I2C_Send_Byte(0xa0);
		AT24Cxx_I2C_Wait_Ack();             //等待应答
		AT24Cxx_I2C_Send_Byte(addr>>8);     //地址的高位
	}
	else 
	{
		AT24Cxx_I2C_Send_Byte(0xa0+ ((addr/256)<<1) );//数据地址
	}
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(addr%256);//地址的地低位
	AT24Cxx_I2C_Wait_Ack();
	
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(dat>>8);//发送高字节
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(dat&0xff);//发送低字节
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Stop();
	Delay_us(10000);
}
/*******************I2C驱动*************************************/
void AT24Cxx_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin=AT24Cxx_I2C_SCL|AT24Cxx_I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(AT24Cxx_GPIO_I2C,&GPIO_InitStructure);

	AT24Cxx_I2C_SCL_H;
	AT24Cxx_I2C_SDA_H;
}

/*******************************************************************************
* 函 数 名         : I2C_SDA_OUT
* 函数功能		   : SDA输出配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void AT24Cxx_I2C_SDA_OUT()
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=AT24Cxx_I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : I2C_SDA_IN
* 函数功能		   : SDA输入配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void AT24Cxx_I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=AT24Cxx_I2C_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//产生起始信号
void AT24Cxx_I2C_Start(void)
{
    AT24Cxx_I2C_SDA_OUT();
	
	AT24Cxx_I2C_SDA_H;
	AT24Cxx_I2C_SCL_H;
	Delay_us(5);
	AT24Cxx_I2C_SDA_L;
	Delay_us(6);
	AT24Cxx_I2C_SCL_L;
}

//产生停止信号
void AT24Cxx_I2C_Stop(void)
{
   AT24Cxx_I2C_SDA_OUT();

   AT24Cxx_I2C_SCL_L;
   AT24Cxx_I2C_SDA_L;
   AT24Cxx_I2C_SCL_H;
   Delay_us(6);
   AT24Cxx_I2C_SDA_H;
   Delay_us(6);
}

//主机产生应答信号ACK
void AT24Cxx_I2C_Ack(void)
{
   AT24Cxx_I2C_SCL_L;
   AT24Cxx_I2C_SDA_OUT();
   AT24Cxx_I2C_SDA_L;
   Delay_us(2);
   AT24Cxx_I2C_SCL_H;
   Delay_us(5);
   AT24Cxx_I2C_SCL_L;
}

//主机不产生应答信号NACK
void AT24Cxx_I2C_NAck(void)
{
   AT24Cxx_I2C_SCL_L;
   AT24Cxx_I2C_SDA_OUT();
   AT24Cxx_I2C_SDA_H;
   Delay_us(2);
   AT24Cxx_I2C_SCL_H;
   Delay_us(5);
   AT24Cxx_I2C_SCL_L;
}
//等待从机应答信号
//返回值：1 接收应答失败
//		  0 接收应答成功
u8 AT24Cxx_I2C_Wait_Ack(void)
{
	u8 tempTime=0;

	AT24Cxx_I2C_SDA_IN();

	AT24Cxx_I2C_SDA_H;
	Delay_us(1);
	AT24Cxx_I2C_SCL_H;
	Delay_us(1);

	while(GPIO_ReadInputDataBit(AT24Cxx_GPIO_I2C,AT24Cxx_I2C_SDA))
	{
		tempTime++;
		if(tempTime>250)
		{
			AT24Cxx_I2C_Stop();
			return 1;
		}	 
	}

	AT24Cxx_I2C_SCL_L;
	return 0;
}
//I2C 发送一个字节
void AT24Cxx_I2C_Send_Byte(u8 txd)
{
	u8 i=0;

	AT24Cxx_I2C_SDA_OUT();
	AT24Cxx_I2C_SCL_L;//拉低时钟开始数据传输

	for(i=0;i<8;i++)
	{
		if((txd&0x80)>0) //0x80  1000 0000
			AT24Cxx_I2C_SDA_H;
		else
			AT24Cxx_I2C_SDA_L;

		txd<<=1;
		AT24Cxx_I2C_SCL_H;
		Delay_us(2); //发送数据
		AT24Cxx_I2C_SCL_L;
		Delay_us(2);
	}
}

//I2C 读取一个字节

u8 AT24Cxx_I2C_Read_Byte(u8 ack)
{
   u8 i=0,receive=0;

   AT24Cxx_I2C_SDA_IN();
   for(i=0;i<8;i++)
   {
   		AT24Cxx_I2C_SCL_L;
		Delay_us(2);
		AT24Cxx_I2C_SCL_H;
		receive<<=1;
		if(GPIO_ReadInputDataBit(AT24Cxx_GPIO_I2C,AT24Cxx_I2C_SDA))
		   receive++;
		Delay_us(1);	
   }

   	if(ack==0)
	   	AT24Cxx_I2C_NAck();
	else
		AT24Cxx_I2C_Ack();

	return receive;
}
