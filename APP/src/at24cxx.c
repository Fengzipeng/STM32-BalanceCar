#include "at24cxx.h"
u8 AT24Cxx_ReadOneByte(u16 addr)//��ȡһ���ֽ�
{                         //add--���ݵ�ַ
	u8 temp;
	AT24Cxx_I2C_Start();
  AT24Cxx_I2C_Send_Byte(0xa0);
	AT24Cxx_I2C_Wait_Ack();             //�ȴ�Ӧ��
	AT24Cxx_I2C_Send_Byte(addr>>8);
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(addr%256);
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(0xa1);
	AT24Cxx_I2C_Wait_Ack();
	temp=AT24Cxx_I2C_Read_Byte(0);//��Ӧ��
	AT24Cxx_I2C_Stop();
	return temp;
}
void AT24Cxx_WriteOneByte(u16 addr,u8 dat)
{
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(0xa0);
	AT24Cxx_I2C_Wait_Ack();             //�ȴ�Ӧ��
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
u16 AT24Cxx_ReadTwoByte(u16 addr)//�����ֽ�
{
	u16 temp;
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(0xa0);
	AT24Cxx_I2C_Wait_Ack();             //�ȴ�Ӧ��
	AT24Cxx_I2C_Send_Byte(addr>>8);
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(addr%256);
	AT24Cxx_I2C_Wait_Ack();
	
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(0xa1);
	AT24Cxx_I2C_Wait_Ack();
	temp=AT24Cxx_I2C_Read_Byte(1);//��Ӧ��
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
		AT24Cxx_I2C_Wait_Ack();             //�ȴ�Ӧ��
		AT24Cxx_I2C_Send_Byte(addr>>8);     //��ַ�ĸ�λ
	}
	else 
	{
		AT24Cxx_I2C_Send_Byte(0xa0+ ((addr/256)<<1) );//���ݵ�ַ
	}
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(addr%256);//��ַ�ĵص�λ
	AT24Cxx_I2C_Wait_Ack();
	
	AT24Cxx_I2C_Start();
	AT24Cxx_I2C_Send_Byte(dat>>8);//���͸��ֽ�
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Send_Byte(dat&0xff);//���͵��ֽ�
	AT24Cxx_I2C_Wait_Ack();
	AT24Cxx_I2C_Stop();
	Delay_us(10000);
}
/*******************I2C����*************************************/
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
* �� �� ��         : I2C_SDA_OUT
* ��������		   : SDA�������	   
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : I2C_SDA_IN
* ��������		   : SDA��������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void AT24Cxx_I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=AT24Cxx_I2C_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//������ʼ�ź�
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

//����ֹͣ�ź�
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

//��������Ӧ���ź�ACK
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

//����������Ӧ���ź�NACK
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
//�ȴ��ӻ�Ӧ���ź�
//����ֵ��1 ����Ӧ��ʧ��
//		  0 ����Ӧ��ɹ�
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
//I2C ����һ���ֽ�
void AT24Cxx_I2C_Send_Byte(u8 txd)
{
	u8 i=0;

	AT24Cxx_I2C_SDA_OUT();
	AT24Cxx_I2C_SCL_L;//����ʱ�ӿ�ʼ���ݴ���

	for(i=0;i<8;i++)
	{
		if((txd&0x80)>0) //0x80  1000 0000
			AT24Cxx_I2C_SDA_H;
		else
			AT24Cxx_I2C_SDA_L;

		txd<<=1;
		AT24Cxx_I2C_SCL_H;
		Delay_us(2); //��������
		AT24Cxx_I2C_SCL_L;
		Delay_us(2);
	}
}

//I2C ��ȡһ���ֽ�

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
