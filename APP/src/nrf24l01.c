#include "nrf24l01.h"
#include "spi.h"
u8 RX_Buff[3];
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};
//��ʼ��24L01��IO��
void NRF24L01_Init(u8 Mode)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;		  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//CE
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��ָ��IO
	NRF_CE_LOW;       //���� �������ģʽ

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//CSN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	NRF_CSN_HIGH;//����		��ֹSPIͨ��		
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;   //IRQ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PG13�������� 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
  SPI1_Init();    		//��ʼ��SPI	 
	NRF_CSN_HIGH;			//SPIƬѡȡ��  
	
  while(NRF24L01_Check()!=1)
	{
		LED2_ON;
	}
	if(Mode==NRF24L01_TXMode)  NRF24L01_ModeSet(NRF24L01_TXMode);
	else  NRF24L01_ModeSet(NRF24L01_RXMode);
}

//���24L01�Ƿ����
//����ֵ:1���ɹ�;0��ʧ��	
u8 NRF24L01_Check()
{
	u8 buf[5]={0x5a,0xa5,0xc0,0xa3,0x9f};
  u8 buf1[5];
	u8 i;  	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)
	  {
			if(buf1[i]!=buf[i]) break;	
		}			
	if(i!=5)return 0;//���24L01����	
	return 1;		 //��⵽24L01
}

//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	  u8 status;	
   	NRF_CSN_LOW;                 //ʹ��SPI����
  	status =SPI1_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	NRF_CSN_HIGH;                 //��ֹSPI����	   
  	return status;       			//����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 		NRF_CSN_LOW;          //ʹ��SPI����		
  	SPI1_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI1_ReadWriteByte(0X00);//��ȡ�Ĵ�������
  	NRF_CSN_HIGH;           //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	

//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,i;	       
  	NRF_CSN_LOW;            //ʹ��SPI����
  	status=SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	  for(i=0;i<len;i++)
	     {
				 *pBuf=SPI1_ReadWriteByte(0X00);//��������
				 pBuf++;
			 }
  	NRF_CSN_HIGH;          //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	  NRF_CSN_LOW;          //ʹ��SPI����
  	status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	 {
	       SPI1_ReadWriteByte(*pBuf); //д������	
		     pBuf++;		 
	 }
  	NRF_CSN_HIGH;        //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}				



//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
	

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{
	  NRF_CE_LOW;	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07);//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG_NRF24L01, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	NRF_CE_HIGH;	 //CEΪ��,�������ģʽ 
}					

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
	  NRF_CE_LOW;	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG_NRF24L01,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	  NRF_CE_HIGH;//CEΪ��,10us����������
}
u8 NRF24L01_Send_Data(u8 *txBuf) //����32�ֽڵ�����  �����óɷ���ģʽ
{
	  u8 status,time_1=0;
	  NRF_CE_LOW;
		NRF24L01_Write_Buf(WR_TX_PLOAD,txBuf,TX_PLOAD_WIDTH);
		NRF_CE_HIGH;//���߷�������
	  while(NRF_IRQ_Status!=0)
	 {   
		  Delay_us(5);
		  time_1++;
   		 if(time_1==10)
			 {
				 NRF24L01_Write_Reg(FLUSH_TX,0xff);//50us��û�з��ͳɹ����������
				 return 0;                             //�˳�
			 }
		}
	  status=NRF24L01_Read_Reg(STATUS);
	 if((status&TX_OK))//������ɻ��ߴﵽ����ʹ���
	 {
		 NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,0xff);//����жϱ�־λ
		 NRF24L01_Write_Reg(FLUSH_TX,0xff);       //��ϴ������
		 return TX_OK;//���ͳɹ�
	 }
	 if(status&MAX_TX)//������ɻ��ߴﵽ����ʹ���
	 {
		 NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,0xff);//����жϱ�־λ
		 NRF24L01_Write_Reg(FLUSH_TX,0xff);       //��ϴ������
		 return MAX_TX;//����ʧ�� ���ؼĴ�����ֵ
	 }
	 return 0; //δ֪ԭ����ʧ��
}
u8 NRF24L01_ReceiveData(u8 *rxBuf)
{
	u8 status;
	status=NRF24L01_Read_Reg(STATUS);
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,0xff);
	if(status&RX_OK)
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxBuf,RX_PLOAD_WIDTH);
		NRF24L01_Write_Reg(FLUSH_RX,0xff); 
		return status;  //���ճɹ�����״̬
	}
	return 0; //����ʧ��
}
void NRF24L01_ModeSet(u8 mode )
{
	if(mode==NRF24L01_TXMode) NRF24L01_TX_Mode();
	if(mode==NRF24L01_RXMode) NRF24L01_RX_Mode();
}



	 	 

