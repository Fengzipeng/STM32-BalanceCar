#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H
#include "stm32f10x.h"
extern unsigned char DataScope_OutPut_Buffer[42];	   //������֡���ݻ�����
extern float Send_Data_Buf[10];

void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    // дͨ�������� ������֡���ݻ�����

unsigned char DataScope_Data_Generate(unsigned char Channel_Number);  // ����֡�������ɺ��� 
void Send(u8 Channel_Sum,u8 USART_Num);//��������
#define Channel_1 1
#define Channel_2 2
#define Channel_3 3
#define Channel_4 4
#define Channel_5 5
#define Channel_6 6
#define Channel_7 7
#define Channel_8 8
#define Channel_9 9
#define Channel_10 10
#endif 



