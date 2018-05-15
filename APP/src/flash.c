#include "flash.h"
//#include "malloc.h"

void FLASH_SectorErase(uint32_t eraseAddr);
void FLASH_WriteEnable(void);
u8 FLASH_CheckBusy(void);
void FLASH_WritePage(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum);

u16 FLASH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	/************SPI2*************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/***************SPI_CS**************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12); 
    /* 初始化SPI2 */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//选择全双工SPI模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位SPI
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //时钟悬空高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;      //在第二个时钟采集数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nss使用软件控制
	/* 选择波特率预分频为256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//从最高位开始传输
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Cmd(SPI2, ENABLE);
	SPI_Init(SPI2, &SPI_InitStructure);
    /* 初始化FLASH要读取芯片ID一下 */
	return(FLASH_ReadID());   
}

u16 FLASH_ReadID(void)
{
	u16 id = 0;
	
	FLASH_CS_CLR;            //打开片选
	
	SPI2_WriteReadData(0x90);//发送读取ID命令	    
	SPI2_WriteReadData(0x00); 	    
	SPI2_WriteReadData(0x00); 	    
	SPI2_WriteReadData(0x00);

	id |= SPI2_WriteReadData(0xFF) << 8; //读取16位ID 
	id |= SPI2_WriteReadData(0xFF);
		 
	FLASH_CS_SET;			 //关闭片选   
	return id;	
}

/****************************************************************************
* Function Name  : FLASH_CheckBusy
* Description    : 检测FLASH是否忙碌.
* Input          : None
* Output         : None
* Return         : 0：不忙碌。0xFF：等待超时。
****************************************************************************/
u8 FLASH_CheckBusy(void)
{
	u8 statusValue;
	u32 timeCount = 0u;
	
	do
	{
		timeCount++;
		if(timeCount > 0xEFFFFFFF)				   //等待超时
		{
			return 0xFF;
		}
		FLASH_CS_CLR;                              //使能器件
		   
		SPI2_WriteReadData(EN25X_ReadStatusReg);   //发送读取状态寄存器命令    
		statusValue = SPI2_WriteReadData(0xFF);    //读取一个字节  
	
		FLASH_CS_SET;                              //取消片选
	}
	while((statusValue & 0x01) == 0x01);    	   // 等待BUSY位清空

	return 0; 
}

/****************************************************************************
* Function Name  : FLASH_ReadData
* Description    : 读取FLASH的数据.
* Input          : readBuff：读取缓存器
*                * readAddr：读取地址
*                * readByteNum：读取数据长度
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_ReadData(uint8_t *readBuff, uint32_t readAddr, uint16_t readByteNum)
{
  SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
  FLASH_CheckBusy();				      //确定FLASH是否处于忙状态
	FLASH_CS_CLR;    //打开片选
	/* 写读取命令 */
	SPI2_WriteReadData(EN25X_ReadData);
	/* 发送24位读取地址 */
	SPI2_WriteReadData(readAddr >> 16);
	SPI2_WriteReadData(readAddr >> 8);
	SPI2_WriteReadData(readAddr);
	/* 读取数据 */
	while(readByteNum--)
	{
		*readBuff = SPI2_WriteReadData(0xFF);
		readBuff++;
	}
	FLASH_CS_SET;   //关闭片选
}

/****************************************************************************
* Function Name  : FLASH_WriteEnable
* Description    : 写使能
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
void FLASH_WriteEnable(void)
{
	FLASH_CS_CLR;                               //使能器件  
  SPI2_WriteReadData(EN25X_WriteEnable);       //发送写使能 
	FLASH_CS_SET;                               //取消片选     	      	
}

/****************************************************************************
* Function Name  : FLASH_SectorErase
* Description    : 扇区擦除
* Input          : eraseAddr：扇区地址
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_SectorErase(uint32_t eraseAddr)
{		
	FLASH_CheckBusy();	 //确定FLASH是否处于忙状态
    FLASH_WriteEnable(); //开启写使能

	FLASH_CS_CLR;        //使能器件

	SPI2_WriteReadData(EN25X_SectorErase); //发送命令

	SPI2_WriteReadData(eraseAddr >> 16);  //发送24位地址
	SPI2_WriteReadData(eraseAddr >> 8);
	SPI2_WriteReadData(eraseAddr);

	FLASH_CS_SET;                         //取消片选     	      	 
}

/****************************************************************************
* Function Name  : FLASH_ChipErase
* Description    : 整片擦除
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_ChipErase(void)
{
	FLASH_CheckBusy();	 //确定FLASH是否处于忙状态
	FLASH_WriteEnable(); //开启写使能

	FLASH_CS_CLR;        //使能器件

	SPI2_WriteReadData(EN25X_ChipErase); //发送命令

	FLASH_CS_SET;                         //取消片选
}

/****************************************************************************
* Function Name  : FLASH_WritePage
* Description    : 向空的FLASH里面写数据（注意是空的！）
* Input          : writeBuff：写缓存器
*                * writeAddr：写入地址
*                * writeByteNum：写入数据长度
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_WritePage(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum)
{
	uint16_t byteNum, i;

	byteNum = writeAddr % 256;	  
	byteNum = 256 - byteNum;		  //求出首页剩余地址
	if(writeByteNum <= byteNum)	      //当写入字节少于首页剩余地址
	{
		byteNum = writeByteNum;
	}		
	/* 开始写入 */
	while(1)
	{  
		/* 写入数据 */
		FLASH_CheckBusy();				      //确定FLASH是否处于忙状态
		FLASH_WriteEnable();		          //开启写使能
		FLASH_CS_CLR;                         //使能器件 
		SPI2_WriteReadData(EN25X_PageProgram); //发送写命令
		SPI2_WriteReadData(writeAddr >> 16);  //发送24位读取地址
		SPI2_WriteReadData(writeAddr >> 8);
		SPI2_WriteReadData(writeAddr);
	    for(i=0; i<byteNum; i++)				  //循环写数
		{
			SPI2_WriteReadData(*writeBuff);
			writeBuff++;	
		}
		FLASH_CS_SET;                         //取消片选 
		/* 判断是否写完 */
		if(writeByteNum == byteNum) //如果写入字节数等于剩余字节数表示写入完成	
		{
			break;
		}
		else	                    //如果未写入完成
		{
			writeAddr += byteNum;					 //写入地址偏移
			writeByteNum = writeByteNum - byteNum;	 //求出剩余字节数

			if(writeByteNum >= 256)	//如果剩余字节数大于256，那么一次写入一页
			{
				byteNum = 256;	
			}
			else					//如果剩余字节数小于256，那么一次全部写完
			{
				byteNum = writeByteNum;
			}
		}
	} 	
}

/****************************************************************************
* Function Name  : FLASH_WriteData
* Description    : 向FLASH里面写数据（FLASH可以不为空）
* Input          : writeBuff：写缓存器
*                * writeAddr：写入地址
*                * writeByteNum：写入数据长度
* Output         : None
* Return         : None
****************************************************************************/
#ifndef __MALLOC_H
u8 flashBuff[4096]; //读取缓存器（不知道为什么不能放到子函数里面。）
#endif
void FLASH_WriteData(u8 *writeBuff, u32 writeAddr, u16 writeByteNum)
{
	
	uint16_t sectorAddr, byteAddr, byte, i;

  SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	sectorAddr = writeAddr / 4096;	   //扇区地址
	byteAddr = writeAddr % 4096;	   //扇区偏移地址
	byte = 4096 - byteAddr;            //扇区剩余地址

	if(writeByteNum < byte)	   //如果写入字节小于扇区剩余空间(小于一个扇区)
	{
		byte = writeByteNum;
	}
	while(1)
	{

    FLASH_ReadData(flashBuff, sectorAddr*4096, 4096); //读取一个扇区的数据
		for(i=0; i<byte; i++)	  		//检测要写入的地址是不是空的
		{      
			if(flashBuff[byteAddr + i] != 0xFF)	//如果不是
			{
				break;
			} 		
		}

		if(i < byte) //如果写入地址里面有数据
		{
			FLASH_SectorErase(sectorAddr*4096); //擦除这个扇区
			
			for(i=0; i<byte; i++)				//复制数据
			{
			flashBuff[byteAddr + i] = writeBuff[i];
			} 
			FLASH_WritePage(flashBuff, sectorAddr*4096, 4096);		
	}
		else		 //要写入的地址内无数据
		{
			FLASH_WritePage(writeBuff, writeAddr, byte);			
		}

		/* 判断是否写完 */
		if(byte == writeByteNum)		//如果写入字节数等于剩余字节数表示写入完成
		{
			break;
		}
		else                        //如果未写完
		{
			writeBuff += byte;		//数据指针偏移
			writeAddr += byte;		//求出写入地址偏移
			writeByteNum -= byte;   //求出写入数据剩余数
			sectorAddr++;			//扇区地址+1
			byteAddr = 0;           //扇区偏移地址为0

			if(writeByteNum >= 4096)
			{
				byte = 4096;	
			}
			else
			{
				byte = writeByteNum;
			}		
		}
	}  	 				
}
/*********************SPI操作函数*******************************/
void SPI2_SetSpeed(u8 Speed)
{
	SPI2->CR1 &= 0xFFC7; 
	SPI2->CR1 |= Speed;
	SPI_Cmd(SPI2,ENABLE);
}
u8 SPI2_WriteReadData(u8 dat)
{
	u16 i = 0;
    /* 当发送缓冲器空 */	
 	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFF;
		}
	}
    /* 发送数据 */
   	SPI_I2S_SendData(SPI2, dat);
	/* 等待接收缓冲器为非空 */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};
 	/* 将读取到的数值返回 */
 	return SPI_I2S_ReceiveData(SPI2);		
}
