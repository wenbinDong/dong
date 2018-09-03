/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                      POWER MANAGEMENT PRODUCT
*
* Filename      : bsp_adc.c
* Version       : V2.00
* Programmer(s) : XU
*********************************************************************************************************
*/ 

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "bsp_sw.h"
#include "bsp_dac.h"
#include "bsp_SysTick.h"

/*
*********************************************************************************************************
*                                            VARIABLES
*********************************************************************************************************
*/

#define REF_VOL	3000 
#define	REF_RES	16      //²Î¿¼ÄÚ×è1.6mO  define in bsp_adc.h document

void BAT_SW_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(BSP_BAT_SG_RCC_APB2Periph|BSP_BAT_SW2_RCC_APB2Periph|BSP_BAT_SEL_RCC_APB2Periph\
												|BSP_BAT_SW1_RCC_APB2Periph, ENABLE );
															   
	GPIO_InitStructure.GPIO_Pin = BSP_BAT1_SG_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BSP_BAT1_SG_GPIO, &GPIO_InitStructure);
															   
	GPIO_InitStructure.GPIO_Pin = BSP_BAT1_SW2_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BSP_BAT1_SW2_GPIO, &GPIO_InitStructure);
																   
	GPIO_InitStructure.GPIO_Pin = BSP_BAT1_SEL_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BSP_BAT1_SEL_GPIO, &GPIO_InitStructure);
														   
	GPIO_InitStructure.GPIO_Pin = BSP_BAT1_SW1_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BSP_BAT1_SW1_GPIO, &GPIO_InitStructure);
	
	//initial is low leavl
	BAT1_SG_OFF;
	BAT1_SW2_OFF;
	BAT1_SW1_OFF;
	BAT1_SEL_OFF;
}
