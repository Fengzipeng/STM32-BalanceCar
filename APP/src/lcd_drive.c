/* type change */
typedef unsigned char       bool;
typedef unsigned char       u8;
typedef unsigned short      u16;
#define  True  1
#define  False 0

//SET BIT.    Example: a |= SETBIT0
enum
{
	SETBIT0 = 0x0001,  SETBIT1 = 0x0002,	SETBIT2 = 0x0004,	 SETBIT3 = 0x0008,
	SETBIT4 = 0x0010,	 SETBIT5 = 0x0020,	SETBIT6 = 0x0040,	 SETBIT7 = 0x0080,
	SETBIT8 = 0x0100,	 SETBIT9 = 0x0200,	SETBIT10 = 0x0400, SETBIT11 = 0x0800,
	SETBIT12 = 0x1000, SETBIT13 = 0x2000,	SETBIT14 = 0x4000, SETBIT15 = 0x8000		
};
//CLR BIT.    Example: a &= CLRBIT0
enum
{
	CLRBIT0 = 0xFFFE,  CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	 CLRBIT3 = 0xFFF7,	
	CLRBIT4 = 0xFFEF,	 CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	 CLRBIT7 = 0xFF7F,
	CLRBIT8 = 0xFEFF,	 CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF, CLRBIT11 = 0xF7FF,
	CLRBIT12 = 0xEFFF, CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF, CLRBIT15 = 0x7FFF
};
//CHOSE BIT.  Example: a = b&CHSBIT0
enum
{
	CHSBIT0 = 0x0001,  CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,	 CHSBIT3 = 0x0008,
	CHSBIT4 = 0x0010,	 CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	 CHSBIT7 = 0x0080,
	CHSBIT8 = 0x0100,	 CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400, CHSBIT11 = 0x0800,
	CHSBIT12 = 0x1000, CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000, CHSBIT15 = 0x8000		
};

/* INCLUDES */
//MCU
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_fsmc.h"
//Derive
#include "lcd_driver.h"

//LCD地址结构体
typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;		    
#define	 LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define  LCD             ((LCD_TypeDef *) LCD_BASE)

/*-------------------------------------------------------------------------------------------------------
*  函数声明												 
-------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------
*  函数声明												 
-------------------------------------------------------------------------------------------------------*/
//mcu
void Driver_MCU_Init(void);
//lcd
void Driver_LcdBacklight(bool sta);
void Driver_LcdReset(bool sta);
void Driver_LcdCS(bool sta);
void Driver_LcdRS(bool sta);
void Driver_LcdRD(bool sta);
void Driver_LcdWR(bool sta);
void Driver_LcdSendData(u16 Temp);

/*-------------------------------------------------------------------------------------------------------
*  执行代码													 
-------------------------------------------------------------------------------------------------------*/
//-------------------------------------------------------------------------------------------------------
//-------------------------------------- LCD DRIVER -----------------------------------------------------
//-------------------------------------------------------------------------------------------------------
/********************************************************************************************************
*  Function: Driver_LcdBacklight				                                                           
*  Object: lcd backlight control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdBacklight(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PB0
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(sta)
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
		else
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

/********************************************************************************************************
*  Function: Driver_LcdReset				                                                           
*  Object: lcd reset control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdReset(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PC5
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		else
			GPIO_SetBits(GPIOC, GPIO_Pin_5);
}

/********************************************************************************************************
*  Function: Driver_LcdCS		                                                           
*  Object: lcd enable/disable control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdCS(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PG12
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOG, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOG, GPIO_Pin_12);
		else
			GPIO_SetBits(GPIOG, GPIO_Pin_12);
}

/********************************************************************************************************
*  Function: Driver_LcdRS		                                                           
*  Object: lcd RS(cmd/data chose) control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdRS(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PG0
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOG, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOG, GPIO_Pin_0);
		else
			GPIO_SetBits(GPIOG, GPIO_Pin_0);
}

/********************************************************************************************************
*  Function: Driver_LcdRD		                                                           
*  Object: lcd read control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdRD(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PD4
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOD, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOD, GPIO_Pin_4);
		else
			GPIO_SetBits(GPIOD, GPIO_Pin_4);
}

/********************************************************************************************************
*  Function: Driver_LcdWR		                                                           
*  Object: lcd write control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdWR(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PD5
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOD, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOD, GPIO_Pin_5);
		else
			GPIO_SetBits(GPIOD, GPIO_Pin_5);
}

/********************************************************************************************************
*  Function: Driver_LcdSendData		                                                           
*  Object: lcd data send
*  Input: Temp
*  Output: none                                  
*  brief: len is 16bits
********************************************************************************************************/
void Driver_LcdSendData(u16 Temp)
{
		//PD10 PD9 PD8    PE15 PE14 PE13 PE12 PE11 PE10 PE9 PE8 PE7     PD1 PD0 PD15 PD14
		static bool StartFlag = True;
		if(StartFlag)
		{
				GPIO_InitTypeDef GPIO_InitStructure;
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_1 | GPIO_Pin_0 | GPIO_Pin_15 | GPIO_Pin_14;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
				GPIO_Init(GPIOD, &GPIO_InitStructure);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
				GPIO_Init(GPIOE, &GPIO_InitStructure);
				StartFlag = False;
		}		
		/*
		if(Temp&CHSBIT15)
			GPIO_SetBits(GPIOD, GPIO_Pin_10);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_10);
		if(Temp&CHSBIT14)
			GPIO_SetBits(GPIOD, GPIO_Pin_9);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_9);
		if(Temp&CHSBIT13)
			GPIO_SetBits(GPIOD, GPIO_Pin_8);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_8);
		if(Temp&CHSBIT12)
			GPIO_SetBits(GPIOE, GPIO_Pin_15);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_15);
		if(Temp&CHSBIT11)
			GPIO_SetBits(GPIOE, GPIO_Pin_14);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_14);
		if(Temp&CHSBIT10)
			GPIO_SetBits(GPIOE, GPIO_Pin_13);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_13);
		if(Temp&CHSBIT9)
			GPIO_SetBits(GPIOE, GPIO_Pin_12);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_12);
		if(Temp&CHSBIT8)
			GPIO_SetBits(GPIOE, GPIO_Pin_11);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_11);
		if(Temp&CHSBIT7)
			GPIO_SetBits(GPIOE, GPIO_Pin_10);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_10);
		if(Temp&CHSBIT6)
			GPIO_SetBits(GPIOE, GPIO_Pin_9);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
		if(Temp&CHSBIT5)
			GPIO_SetBits(GPIOE, GPIO_Pin_8);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);
		if(Temp&CHSBIT4)
			GPIO_SetBits(GPIOE, GPIO_Pin_7);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_7);
		if(Temp&CHSBIT3)
			GPIO_SetBits(GPIOD, GPIO_Pin_1);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_1);
		if(Temp&CHSBIT2)
			GPIO_SetBits(GPIOD, GPIO_Pin_0);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_0);
		if(Temp&CHSBIT1)
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		if(Temp&CHSBIT0)
			GPIO_SetBits(GPIOD, GPIO_Pin_14);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		*/
		GPIO_SetBits(GPIOD, ((Temp)/0x2000)<<8 & 0x0700);
		GPIO_ResetBits(GPIOD, (~((Temp)/0x2000)<<8 & 0x0700));
		GPIO_SetBits(GPIOE, ((Temp<<3)/0x80)<<7 & 0xff80);
		GPIO_ResetBits(GPIOE, (~((Temp<<3)/0x80)<<7 & 0xff80));
		GPIO_SetBits(GPIOD, ((Temp<<12)/0x4000) & 0x0003);
		GPIO_ResetBits(GPIOD, (~((Temp<<12)/0x4000) & 0x0003));
		GPIO_SetBits(GPIOD, ((Temp<<14)/0x4000)<<14 & 0xC000);
		GPIO_ResetBits(GPIOD, (~((Temp<<14)/0x4000)<<14 & 0xC000));
		
}

////-------------------------------------- SPI DRIVER -----------------------------------------------------
/********************************************************************************************************
*  Function: Driver_LcdFSMCON				                                                           
*  Object: FSMC init
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCON(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
		FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
		FSMC_NORSRAMTimingInitTypeDef  writeTiming;
		//使能FSMC时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		//PORTD复用推挽输出  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;			
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure); 
		//PORTE复用推挽输出  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE, &GPIO_InitStructure); 
   	//PORTG复用推挽输出  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOG, &GPIO_InitStructure); 
		//FSMC时序设置
		readWriteTiming.FSMC_AddressSetupTime = 0x01;	
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 
    readWriteTiming.FSMC_DataSetupTime = 0x0f;	
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	
	  writeTiming.FSMC_AddressSetupTime = 0x02;
    writeTiming.FSMC_AddressHoldTime = 0x00;		
    writeTiming.FSMC_DataSetupTime = 0x05;	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 
		//LCD配置
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; 
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming; 
		//初始化FSMC 使能BANK
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  
   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}

/********************************************************************************************************
*  Function: Driver_LcdFSMCWriteReg				                                                           
*  Object: 通过FSMC接口写REG
*  Input: Index
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCWriteReg(u16 Index)
{
		LCD->LCD_REG = Index;
}

/********************************************************************************************************
*  Function: Driver_LcdFSMCWriteData				                                                           
*  Object: 通过FSMC接口写Data
*  Input: Data
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCWriteData(u16 Data)
{
		LCD->LCD_RAM = Data;
}

/********************************************************************************************************
*  Function: Driver_LcdFSMCReadData				                                                           
*  Object: 通过FSMC接口读Data
*  Input: none
*  Output: Data                                  
*  brief: none
********************************************************************************************************/
u16 Driver_LcdFSMCReadData(void)
{
		return (LCD->LCD_RAM);
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------- TOUCH SPI DRIVER -----------------------------------------------
//-------------------------------------------------------------------------------------------------------
/********************************************************************************************************
*  Function: Driver_TouchCS					                                                           
*  Object: touch ic spi enable/unable
*  Input: 1/0
*  Output: none 							                         	                                     
*  brief:	none                                 
********************************************************************************************************/
void Driver_SPICS(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			StartFlag = False;
			//FCS/SDCS UNABLE
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_SetBits(GPIOC, GPIO_Pin_2 | GPIO_Pin_3);
		}	
		if(!sta)
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		else
			GPIO_SetBits(GPIOB, GPIO_Pin_2);
}

/********************************************************************************************************
*  Function: Driver_SPIMISO					                                                           
*  Object: SPI_MOSI
*  Input: none
*  Output: 1/0 							                         	                                     
*  brief:	none                                   
********************************************************************************************************/
bool Driver_SPIMISO(void)
{
		static bool StartFlag = True;
		if(StartFlag)
		{	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 	
			GPIO_Init(GPIOF, &GPIO_InitStructure);
			StartFlag = False;
		}
		return GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8);
}

/********************************************************************************************************
*  Function: Driver_SPIMOSI					                                                           
*  Object: SPI_MOSI
*  Input: 1/0
*  Output: none 							                         	                                     
*  brief:	none                                   
********************************************************************************************************/
void Driver_SPIMOSI(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOF, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		else
			GPIO_SetBits(GPIOF, GPIO_Pin_9);
}

/********************************************************************************************************
*  Function: Driver_SPISCK					                                                           
*  Object: SPI_SCK
*  Input: 1/0
*  Output: none 							                         	                                     
*  brief:	none  	                                  
********************************************************************************************************/
void Driver_SPISCK(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		else
			GPIO_SetBits(GPIOB, GPIO_Pin_1);
}
