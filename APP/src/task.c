#include "task.h"
OS_STK 	LED_TASK_STK[STK_SIZE];
OS_STK 	PRINTF_TASK_STK[STK_SIZE];
OS_STK 	MPU_TASK_STK[STK_SIZE];
OS_STK  AT24Cxx_TASK_STK[STK_SIZE];
OS_STK  KEY_TASK_STK[STK_SIZE];
OS_STK  FLASH_TASK_STK[STK_SIZE];
OS_STK  LCD_TASK_STK[STK_SIZE];
void LCD_Task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	Driver_LcdBacklight(True);
	LCD_ClrScr(BLACK);
	LCD_DisALine(5,5,0,20,RED);
	LCD_DisALine(5,5,1,20,RED);
	LCD_DisALine(215,5,0,20,RED);
	LCD_DisALine(235,5,1,20,RED);
	LCD_DisALine(5,295,1,20,RED);
	LCD_DisALine(5,315,0,20,RED);
	LCD_DisALine(215,315,0,20,RED);
	LCD_DisALine(235,295,1,20,RED);
	//LCD_DisASquare(70,50,100,RED);
    LCD_DisALoop(120,160,50,BLUE);
	LCD_DisASCString16x8(80,40,"Hello World!",BLUE,BLACK);
	LCD_DisASCString16x8(80,56,"Hello World!",BLUE,BLACK);
	while(1)
	{
		OS_ENTER_CRITICAL();
		//LCD_DisAPoint(90,200,RED);
		OS_EXIT_CRITICAL();
		Delay_ms(200);
	}
}
void MPU_Task(void *pdata)
{
	//OS_CPU_SR cpu_sr;
	pdata=pdata;
	while(1)
	{
		Delay_ms(1000);
		Delay_ms(1000);
	}
}
void NRF_GetData(void *pdata)
{
	
}
void Print_Data(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	u8 temp=0;
	//u16 addr=0;
	u8 flash_buff[18];
	u16 row,col;
	pdata=pdata;
	while(1)
	{
		//temp=AT24Cxx_ReadOneByte(addr);
		//FLASH_ReadData(flash_buff,0,18);
		OS_ENTER_CRITICAL();
		Touch_GetSite(&row,&col);
		printf("x=%d   y=%d\n",row,col);
		//printf("%d\n",temp);
		//printf("%d   %d\n",OSIdleCtr,OSCPUUsage);
		//printf("%s\n",flash_buff);
		OS_EXIT_CRITICAL();
		Delay_ms(500);
	}
}
void Usart_SendData(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
  pdata=pdata;
  while(1)
  	{
  	 Delay_ms(1000);
  	}
	
}
void FLASH_Task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	u16 id;
	u8 i;
	static u8 TEXT_Buffer[19]={"FLASH SPI TEST OK!"};
	 pdata=pdata;
	 id=FLASH_ReadID();
	 OS_ENTER_CRITICAL();
	 if(id==0xc216) printf("The Flash ID is %x\n",id);
	 else printf("Flash ERROR");
	 OS_EXIT_CRITICAL();
	while(1)
	{
	 OS_ENTER_CRITICAL();
	 FLASH_WriteData((u8*)TEXT_Buffer, 0, 18);
	 OS_EXIT_CRITICAL();
	 Delay_ms(500);
	}
}
void AT24Cxx_Task(void *pdata)
{
	u16 addr=0;
	u8 value;
	OS_CPU_SR cpu_sr;
	pdata=pdata;
	while(1)
	{
		OS_ENTER_CRITICAL();
	  AT24Cxx_WriteOneByte(addr,value);
		value+=1;
		addr++;
		OS_EXIT_CRITICAL();
		Delay_ms(100);
	}
}
void LED_Task(void *pdata)
{
	pdata=pdata;
	while(1)
	{
		 LED1_ON;
		 Delay_ms(500);
		 LED1_OFF;
		 Delay_ms(500);
	}
}
void KEY_Task(void *pdata)
{
	u8 result=0;
	pdata=pdata;
	while(1)
	{
		result=KEY_PRESS_SCAN(); 
		if(result==1) 
		{
		  LED2_ON;
		  printf("KEY1 Pressed\n");
	    }
		else if(result==2)
    {
		  LED2_ON;
		  printf("KEY2 Pressed\n");
		}
		else LED2_OFF;
		Delay_ms(100);
	}
}
