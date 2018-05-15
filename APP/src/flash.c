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
    /* ��ʼ��SPI2 */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ѡ��ȫ˫��SPIģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λSPI
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //ʱ�����ոߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;      //�ڵڶ���ʱ�Ӳɼ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nssʹ���������
	/* ѡ������Ԥ��ƵΪ256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//�����λ��ʼ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Cmd(SPI2, ENABLE);
	SPI_Init(SPI2, &SPI_InitStructure);
    /* ��ʼ��FLASHҪ��ȡоƬIDһ�� */
	return(FLASH_ReadID());   
}

u16 FLASH_ReadID(void)
{
	u16 id = 0;
	
	FLASH_CS_CLR;            //��Ƭѡ
	
	SPI2_WriteReadData(0x90);//���Ͷ�ȡID����	    
	SPI2_WriteReadData(0x00); 	    
	SPI2_WriteReadData(0x00); 	    
	SPI2_WriteReadData(0x00);

	id |= SPI2_WriteReadData(0xFF) << 8; //��ȡ16λID 
	id |= SPI2_WriteReadData(0xFF);
		 
	FLASH_CS_SET;			 //�ر�Ƭѡ   
	return id;	
}

/****************************************************************************
* Function Name  : FLASH_CheckBusy
* Description    : ���FLASH�Ƿ�æµ.
* Input          : None
* Output         : None
* Return         : 0����æµ��0xFF���ȴ���ʱ��
****************************************************************************/
u8 FLASH_CheckBusy(void)
{
	u8 statusValue;
	u32 timeCount = 0u;
	
	do
	{
		timeCount++;
		if(timeCount > 0xEFFFFFFF)				   //�ȴ���ʱ
		{
			return 0xFF;
		}
		FLASH_CS_CLR;                              //ʹ������
		   
		SPI2_WriteReadData(EN25X_ReadStatusReg);   //���Ͷ�ȡ״̬�Ĵ�������    
		statusValue = SPI2_WriteReadData(0xFF);    //��ȡһ���ֽ�  
	
		FLASH_CS_SET;                              //ȡ��Ƭѡ
	}
	while((statusValue & 0x01) == 0x01);    	   // �ȴ�BUSYλ���

	return 0; 
}

/****************************************************************************
* Function Name  : FLASH_ReadData
* Description    : ��ȡFLASH������.
* Input          : readBuff����ȡ������
*                * readAddr����ȡ��ַ
*                * readByteNum����ȡ���ݳ���
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_ReadData(uint8_t *readBuff, uint32_t readAddr, uint16_t readByteNum)
{
  SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
  FLASH_CheckBusy();				      //ȷ��FLASH�Ƿ���æ״̬
	FLASH_CS_CLR;    //��Ƭѡ
	/* д��ȡ���� */
	SPI2_WriteReadData(EN25X_ReadData);
	/* ����24λ��ȡ��ַ */
	SPI2_WriteReadData(readAddr >> 16);
	SPI2_WriteReadData(readAddr >> 8);
	SPI2_WriteReadData(readAddr);
	/* ��ȡ���� */
	while(readByteNum--)
	{
		*readBuff = SPI2_WriteReadData(0xFF);
		readBuff++;
	}
	FLASH_CS_SET;   //�ر�Ƭѡ
}

/****************************************************************************
* Function Name  : FLASH_WriteEnable
* Description    : дʹ��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
void FLASH_WriteEnable(void)
{
	FLASH_CS_CLR;                               //ʹ������  
  SPI2_WriteReadData(EN25X_WriteEnable);       //����дʹ�� 
	FLASH_CS_SET;                               //ȡ��Ƭѡ     	      	
}

/****************************************************************************
* Function Name  : FLASH_SectorErase
* Description    : ��������
* Input          : eraseAddr��������ַ
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_SectorErase(uint32_t eraseAddr)
{		
	FLASH_CheckBusy();	 //ȷ��FLASH�Ƿ���æ״̬
    FLASH_WriteEnable(); //����дʹ��

	FLASH_CS_CLR;        //ʹ������

	SPI2_WriteReadData(EN25X_SectorErase); //��������

	SPI2_WriteReadData(eraseAddr >> 16);  //����24λ��ַ
	SPI2_WriteReadData(eraseAddr >> 8);
	SPI2_WriteReadData(eraseAddr);

	FLASH_CS_SET;                         //ȡ��Ƭѡ     	      	 
}

/****************************************************************************
* Function Name  : FLASH_ChipErase
* Description    : ��Ƭ����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_ChipErase(void)
{
	FLASH_CheckBusy();	 //ȷ��FLASH�Ƿ���æ״̬
	FLASH_WriteEnable(); //����дʹ��

	FLASH_CS_CLR;        //ʹ������

	SPI2_WriteReadData(EN25X_ChipErase); //��������

	FLASH_CS_SET;                         //ȡ��Ƭѡ
}

/****************************************************************************
* Function Name  : FLASH_WritePage
* Description    : ��յ�FLASH����д���ݣ�ע���ǿյģ���
* Input          : writeBuff��д������
*                * writeAddr��д���ַ
*                * writeByteNum��д�����ݳ���
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_WritePage(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum)
{
	uint16_t byteNum, i;

	byteNum = writeAddr % 256;	  
	byteNum = 256 - byteNum;		  //�����ҳʣ���ַ
	if(writeByteNum <= byteNum)	      //��д���ֽ�������ҳʣ���ַ
	{
		byteNum = writeByteNum;
	}		
	/* ��ʼд�� */
	while(1)
	{  
		/* д������ */
		FLASH_CheckBusy();				      //ȷ��FLASH�Ƿ���æ״̬
		FLASH_WriteEnable();		          //����дʹ��
		FLASH_CS_CLR;                         //ʹ������ 
		SPI2_WriteReadData(EN25X_PageProgram); //����д����
		SPI2_WriteReadData(writeAddr >> 16);  //����24λ��ȡ��ַ
		SPI2_WriteReadData(writeAddr >> 8);
		SPI2_WriteReadData(writeAddr);
	    for(i=0; i<byteNum; i++)				  //ѭ��д��
		{
			SPI2_WriteReadData(*writeBuff);
			writeBuff++;	
		}
		FLASH_CS_SET;                         //ȡ��Ƭѡ 
		/* �ж��Ƿ�д�� */
		if(writeByteNum == byteNum) //���д���ֽ�������ʣ���ֽ�����ʾд�����	
		{
			break;
		}
		else	                    //���δд�����
		{
			writeAddr += byteNum;					 //д���ַƫ��
			writeByteNum = writeByteNum - byteNum;	 //���ʣ���ֽ���

			if(writeByteNum >= 256)	//���ʣ���ֽ�������256����ôһ��д��һҳ
			{
				byteNum = 256;	
			}
			else					//���ʣ���ֽ���С��256����ôһ��ȫ��д��
			{
				byteNum = writeByteNum;
			}
		}
	} 	
}

/****************************************************************************
* Function Name  : FLASH_WriteData
* Description    : ��FLASH����д���ݣ�FLASH���Բ�Ϊ�գ�
* Input          : writeBuff��д������
*                * writeAddr��д���ַ
*                * writeByteNum��д�����ݳ���
* Output         : None
* Return         : None
****************************************************************************/
#ifndef __MALLOC_H
u8 flashBuff[4096]; //��ȡ����������֪��Ϊʲô���ܷŵ��Ӻ������档��
#endif
void FLASH_WriteData(u8 *writeBuff, u32 writeAddr, u16 writeByteNum)
{
	
	uint16_t sectorAddr, byteAddr, byte, i;

  SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	sectorAddr = writeAddr / 4096;	   //������ַ
	byteAddr = writeAddr % 4096;	   //����ƫ�Ƶ�ַ
	byte = 4096 - byteAddr;            //����ʣ���ַ

	if(writeByteNum < byte)	   //���д���ֽ�С������ʣ��ռ�(С��һ������)
	{
		byte = writeByteNum;
	}
	while(1)
	{

    FLASH_ReadData(flashBuff, sectorAddr*4096, 4096); //��ȡһ������������
		for(i=0; i<byte; i++)	  		//���Ҫд��ĵ�ַ�ǲ��ǿյ�
		{      
			if(flashBuff[byteAddr + i] != 0xFF)	//�������
			{
				break;
			} 		
		}

		if(i < byte) //���д���ַ����������
		{
			FLASH_SectorErase(sectorAddr*4096); //�����������
			
			for(i=0; i<byte; i++)				//��������
			{
			flashBuff[byteAddr + i] = writeBuff[i];
			} 
			FLASH_WritePage(flashBuff, sectorAddr*4096, 4096);		
	}
		else		 //Ҫд��ĵ�ַ��������
		{
			FLASH_WritePage(writeBuff, writeAddr, byte);			
		}

		/* �ж��Ƿ�д�� */
		if(byte == writeByteNum)		//���д���ֽ�������ʣ���ֽ�����ʾд�����
		{
			break;
		}
		else                        //���δд��
		{
			writeBuff += byte;		//����ָ��ƫ��
			writeAddr += byte;		//���д���ַƫ��
			writeByteNum -= byte;   //���д������ʣ����
			sectorAddr++;			//������ַ+1
			byteAddr = 0;           //����ƫ�Ƶ�ַΪ0

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
/*********************SPI��������*******************************/
void SPI2_SetSpeed(u8 Speed)
{
	SPI2->CR1 &= 0xFFC7; 
	SPI2->CR1 |= Speed;
	SPI_Cmd(SPI2,ENABLE);
}
u8 SPI2_WriteReadData(u8 dat)
{
	u16 i = 0;
    /* �����ͻ������� */	
 	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFF;
		}
	}
    /* �������� */
   	SPI_I2S_SendData(SPI2, dat);
	/* �ȴ����ջ�����Ϊ�ǿ� */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};
 	/* ����ȡ������ֵ���� */
 	return SPI_I2S_ReceiveData(SPI2);		
}
