#include "stm32f10x.h"
#include "os_includes.h"
#include "os_cpu.h"
#include "os_cfg.h"
#include "ucos_ii.h"
#include "task.h"
#include "public.h"
OS_STK 	START_TASK_STK[STK_SIZE];
void Start_Task(void *pdata);
void SetSysClockTo72(void);
int main(void)
{
	SetSysClockTo72();
	Led_Init();
	Printf_Init();
	AT24Cxx_INIT();
	Key_Init();
	LCD_ili9341Init();
	FLASH_Init();
	OSInit();
  OSTaskCreate(Start_Task,(void *)0,(OS_STK *)&START_TASK_STK[STK_SIZE-1],18);
	SysTick_Init();
	OSStart();
}
void Start_Task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //初始化 统计任务
	OS_ENTER_CRITICAL();
	/*****************************/
	OSTaskCreate(LED_Task,(void *)0,(OS_STK *)&LED_TASK_STK[STK_SIZE-1],9);
	OSTaskCreate(Print_Data,(void *)0,(OS_STK *)&PRINTF_TASK_STK[STK_SIZE-1],10);
	OSTaskCreate(AT24Cxx_Task,(void *)0,(OS_STK *)&AT24Cxx_TASK_STK[STK_SIZE-1],5);
	OSTaskCreate(KEY_Task,(void *)0,(OS_STK *)&KEY_TASK_STK[STK_SIZE-1],7);
	OSTaskCreate(FLASH_Task,(void *)0,(OS_STK *)&FLASH_TASK_STK[STK_SIZE-1],8);
	OSTaskCreate(LCD_Task,(void *)0,(OS_STK *)&LCD_TASK_STK[STK_SIZE-1],11);
	//OSTaskCreate(MPU_Task,(void *)0,(OS_STK *)&MPU_TASK_STK[STK_SIZE-1],6);
	/****************************/
	OSTaskSuspend(18);
	OS_EXIT_CRITICAL();
}
void SetSysClockTo72(void)
{
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if (HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    RCC_PCLK1Config(RCC_HCLK_Div2);

#ifdef STM32F10X_CL
    /* Configure PLLs *********************************************************/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#else
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#endif

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some const to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
}
