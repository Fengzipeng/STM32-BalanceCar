#include "oled.h"
#include "systick.h"
#include "oledfont.h"
#include "control.h"
#include "filter.h"
#include "iic_oled.h"
void OLED_Show()
{
	if(Angle_Balance>=0) 
	{
	   if(Angle_Balance>9) 
	{
			OLED_ShowChar(56,2,' ',16);
			OLED_ShowNum(64,2,Angle_Balance,2,16);
	}
		 else 
		 {
			OLED_ShowChar(56,2,' ',16);
	    OLED_ShowNum(64,2,Angle_Balance,1,16);
			OLED_ShowChar(72,2,' ',16);
		 }
	}
	else 
	{ 
		OLED_ShowChar(56,2,'-',16);
		if(-Angle_Balance>9) 
		{
			OLED_ShowNum(64,2,-Angle_Balance,2,16);
		}
		else
     { 
		  OLED_ShowChar(72,2,' ',16);
		  OLED_ShowNum(64,2,-Angle_Balance,1,16);
		 }
	}
/***************************************************/
		if(Gyro_Balance>=0) 
	{
	   if(Gyro_Balance>9) 
	{
			OLED_ShowChar(56,4,' ',16);
			OLED_ShowNum(64,4,Gyro_Balance,2,16);
	}
		 else 
		 {
			OLED_ShowChar(56,4,' ',16);
	    OLED_ShowNum(64,4,Gyro_Balance,1,16);
			OLED_ShowChar(72,4,' ',16);
		 }
	}
	else 
	{ 
		OLED_ShowChar(56,4,'-',16);
		if(-Gyro_Balance>9) 
		{
			OLED_ShowNum(64,4,-Gyro_Balance,2,16);
		}
		else
     { 
		  OLED_ShowChar(72,4,' ',16);
		  OLED_ShowNum(64,4,-Gyro_Balance,1,16);
		 }
	 }
	/******************************************/
	 if(Balance_Pwm<=0)
	 {
		 OLED_ShowChar(25,0,'-',16);
	   OLED_ShowNum(33,0,Balance_Pwm,4,16);
	 }
	 else 
	 {
		 OLED_ShowChar(25,0,' ',16);
		 OLED_ShowNum(33,0,Balance_Pwm,4,16);
	 }
	 /************************/
	 if(Motor1_PWM<=0)
	 {
		 OLED_ShowChar(33,6,'-',16);
	   OLED_ShowNum(41,6,Motor1_PWM,4,16);
	 }
	 else 
	 {
		 OLED_ShowChar(33,6,' ',16);
		 OLED_ShowNum(41,6,Motor1_PWM,4,16);
	 }
}
void OLED_Init()
{
	IIC_oled_Init();
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭显示
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置命令
	OLED_WR_Byte(0xFF,OLED_CMD);//128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--正常显示
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//打开显示
	OLED_Clear();
}  
void Write_IIC_Command(u8 com)
{
	 IIC_oled_Start();
	 IIC_oled_Send_Byte(0x78);
	 IIC_oled_Wait_Ack();
	 IIC_oled_Send_Byte(0x00);
	 IIC_oled_Wait_Ack();
	 IIC_oled_Send_Byte(com);
	 IIC_oled_Wait_Ack();
	 IIC_oled_Stop();
}
void Write_IIC_Data(u8 dat)
{
	IIC_oled_Start();
	IIC_oled_Send_Byte(0x78);
	IIC_oled_Wait_Ack();
	IIC_oled_Send_Byte(0x40);
	IIC_oled_Wait_Ack();
	IIC_oled_Send_Byte(dat);
	IIC_oled_Wait_Ack();
	IIC_oled_Stop();
}
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	if(cmd)
	{
   Write_IIC_Data(dat);
   }
	else 
 {
   Write_IIC_Command(dat);
	}
}
void OLED_Set_Pos(u8 x, u8 y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X1E,OLED_CMD);  //DISPLAY OFF
}
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
		}
}
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
}
