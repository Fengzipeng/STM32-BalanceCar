#include "ds18b20.h"
void DS18B20_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	GPIO_InitStructure.GPIO_Pin=DQ;		 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(DS_GPIO,&GPIO_InitStructure);
}
void DQ_In()	  // ‰»Î≈‰÷√
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DQ;		 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(DS_GPIO,&GPIO_InitStructure);			
}
void DQ_Out()	  // ‰≥ˆ≈‰÷√
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DQ;		 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(DS_GPIO,&GPIO_InitStructure);			
}
void Ds18b20_Start()
{
	DQ_Out();	  // ‰≥ˆ≈‰÷√
	DS18B20_DQ_L;
	Delay_us(480);
	DS18B20_DQ_H;
	Delay_us(480);	
}
void DS18B20_WriteData(u8 dat)
{
	u8 i=0;
	DQ_Out();	  // ‰≥ˆ≈‰÷√
	for(i=0;i<8;i++)
	{
		DS18B20_DQ_L;
		Delay_us(15);
		if((dat&0x01)==1)
		{
			DS18B20_DQ_H;	
		}
		else
		{
			DS18B20_DQ_L;
		}
		Delay_us(60);
		DS18B20_DQ_H;
		dat>>=1;	
	}
}
u8 DS18B20_ReadData()
{
	u8 i=0,value=0;
	for(i=0;i<8;i++)
	{
		value>>=1;
		DQ_Out();	  // ‰≥ˆ≈‰÷√
		DS18B20_DQ_L;
		Delay_us(4);
		DS18B20_DQ_H;
		Delay_us(10);
		DQ_In();	  // ‰»Î≈‰÷√
		if(GPIO_ReadInputDataBit(DS_GPIO,DQ)==1)
		{
			value|=0x80;
		}
		Delay_us(45);	
	}
	return value;
}
double Read_Temp()
{
	u8 temph,templ;
	u16 temp;
	double value;
	Ds18b20_Start();
	DS18B20_WriteData(0xcc);
	DS18B20_WriteData(0x44);
	Delay_ms(800);
	
	Ds18b20_Start();
	DS18B20_WriteData(0xcc);
	DS18B20_WriteData(0xbe);
	templ=DS18B20_ReadData();
	temph=DS18B20_ReadData();
	temp=temph;
  temp=(temp<<8)+templ;
	if((temp&0xf800)==0xf800)
	{
		temp=(~temp)+1;
		value=temp*(-0.0625);
	}
	else
	{
		value=temp*(0.0625);	
	}
	return value;				
}

