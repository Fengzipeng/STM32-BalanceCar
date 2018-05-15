#include "os_includes.h"
#include "stm32f10x.h"

/****************************************************************************
* Function Name  : SYSYTICK_Init
* Description    : 初始化器件时钟为72MHz，开启系统滴答定时器
* Input          : None
* Output         : None
* Return         : 0：开启成功；1：开启失败
****************************************************************************/

void SYSTICK_Init(void)
{
	uint16_t ms = 0;
	uint32_t reloadValue;
	ms = 1000 / OS_TICKS_PER_SEC;     //计算一次延时多少毫秒
	reloadValue = 72000;              //延时1ms是的重置数
	reloadValue *= ms;                //计算实际的重置数
	if(reloadValue > 0x0FFFFFF)      //重置寄存器只有24位
	{
		reloadValue = 0x0FFFFFF;
	}

	SysTick_Config(reloadValue); //开启成功返回0		
}















