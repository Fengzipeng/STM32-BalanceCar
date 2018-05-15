#include "nrf24l01.h"
#include "spi.h"
u8 RX_Buff[3];
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};
//初始化24L01的IO口
void NRF24L01_Init(u8 Mode)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;		  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//CE
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化指定IO
	NRF_CE_LOW;       //下拉 进入待机模式

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//CSN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化指定IO
 	NRF_CSN_HIGH;//上拉		禁止SPI通信		
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;   //IRQ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PG13上拉输入 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
  SPI1_Init();    		//初始化SPI	 
	NRF_CSN_HIGH;			//SPI片选取消  
	
  while(NRF24L01_Check()!=1)
	{
		LED2_ON;
	}
	if(Mode==NRF24L01_TXMode)  NRF24L01_ModeSet(NRF24L01_TXMode);
	else  NRF24L01_ModeSet(NRF24L01_RXMode);
}

//检测24L01是否存在
//返回值:1，成功;0，失败	
u8 NRF24L01_Check()
{
	u8 buf[5]={0x5a,0xa5,0xc0,0xa3,0x9f};
  u8 buf1[5];
	u8 i;  	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5); //读出写入的地址  
	for(i=0;i<5;i++)
	  {
			if(buf1[i]!=buf[i]) break;	
		}			
	if(i!=5)return 0;//检测24L01错误	
	return 1;		 //检测到24L01
}

//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	  u8 status;	
   	NRF_CSN_LOW;                 //使能SPI传输
  	status =SPI1_ReadWriteByte(reg);//发送寄存器号 
  	SPI1_ReadWriteByte(value);      //写入寄存器的值
  	NRF_CSN_HIGH;                 //禁止SPI传输	   
  	return status;       			//返回状态值
}

//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 		NRF_CSN_LOW;          //使能SPI传输		
  	SPI1_ReadWriteByte(reg);   //发送寄存器号
  	reg_val=SPI1_ReadWriteByte(0X00);//读取寄存器内容
  	NRF_CSN_HIGH;           //禁止SPI传输		    
  	return(reg_val);           //返回状态值
}	

//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,i;	       
  	NRF_CSN_LOW;            //使能SPI传输
  	status=SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
 	  for(i=0;i<len;i++)
	     {
				 *pBuf=SPI1_ReadWriteByte(0X00);//读出数据
				 pBuf++;
			 }
  	NRF_CSN_HIGH;          //关闭SPI传输
  	return status;        //返回读到的状态值
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	  NRF_CSN_LOW;          //使能SPI传输
  	status = SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	 {
	       SPI1_ReadWriteByte(*pBuf); //写入数据	
		     pBuf++;		 
	 }
  	NRF_CSN_HIGH;        //关闭SPI传输
  	return status;          //返回读到的状态值
}				



//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
	

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void NRF24L01_RX_Mode(void)
{
	  NRF_CE_LOW;	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG_NRF24L01, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	NRF_CE_HIGH;	 //CE为高,进入接收模式 
}					

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(void)
{														 
	  NRF_CE_LOW;	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG_NRF24L01,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	  NRF_CE_HIGH;//CE为高,10us后启动发送
}
u8 NRF24L01_Send_Data(u8 *txBuf) //发送32字节的数据  先设置成发送模式
{
	  u8 status,time_1=0;
	  NRF_CE_LOW;
		NRF24L01_Write_Buf(WR_TX_PLOAD,txBuf,TX_PLOAD_WIDTH);
		NRF_CE_HIGH;//拉高发送数据
	  while(NRF_IRQ_Status!=0)
	 {   
		  Delay_us(5);
		  time_1++;
   		 if(time_1==10)
			 {
				 NRF24L01_Write_Reg(FLUSH_TX,0xff);//50us后没有发送成功清除缓冲器
				 return 0;                             //退出
			 }
		}
	  status=NRF24L01_Read_Reg(STATUS);
	 if((status&TX_OK))//发送完成或者达到最大发送次数
	 {
		 NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,0xff);//清除中断标志位
		 NRF24L01_Write_Reg(FLUSH_TX,0xff);       //冲洗缓冲器
		 return TX_OK;//发送成功
	 }
	 if(status&MAX_TX)//发送完成或者达到最大发送次数
	 {
		 NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,0xff);//清除中断标志位
		 NRF24L01_Write_Reg(FLUSH_TX,0xff);       //冲洗缓冲器
		 return MAX_TX;//发送失败 返回寄存器的值
	 }
	 return 0; //未知原因发送失败
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
		return status;  //接收成功返回状态
	}
	return 0; //接收失败
}
void NRF24L01_ModeSet(u8 mode )
{
	if(mode==NRF24L01_TXMode) NRF24L01_TX_Mode();
	if(mode==NRF24L01_RXMode) NRF24L01_RX_Mode();
}



	 	 

