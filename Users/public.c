#include "public.h"
//数据数组
u8 NRF_Received_Data[32]={0};
/*NRF_Received_Data[]*/
void Public()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
}

