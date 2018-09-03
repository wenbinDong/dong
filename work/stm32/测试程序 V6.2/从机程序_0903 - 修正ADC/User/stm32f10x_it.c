/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_sw.h"
#include "bsp_led1.h"
extern void TimingDelay_Decrement(void);

extern __IO uint32_t flag;
extern  CanRxMsg RxMessage;
extern void TimingDelay_Decrement(void);

// ADC1转换的电压值通过MDA方式传到SRAM
//extern __IO uint16_t ADC_ConvertedValueNVIC[FRE_NUM];
__IO uint32_t ADC_ConvertedValueNVIC[FRE_NUM];
__IO uint16_t ADC_ConvertedValue_Ave[FRE_NUM];
__IO uint32_t i=0;
__IO uint32_t i_nvic=0,i_avg=0,j=0,kkk=0;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
	
	
void USB_LP_CAN1_RX0_IRQHandler(void)
{
   
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  /* 比较是否是发送的数据和ID */ 
  if(RxMessage.ExtId==0x1001)
  {
    flag = 1;
  }
  else if(RxMessage.ExtId==0x1002)
  {
    flag = 2;
  }
	else if(RxMessage.ExtId==0x1003)
  {
    flag = 3;
  }
	else if(RxMessage.ExtId==0x1004)
  {
    flag = 4;
  }
	else
	{
		flag=0xff;
	}
}


void ADC1_2_IRQHandler(void)
{
	int i;
	
  if (ADC_GetITStatus(ADC1,ADC_IT_EOC)==SET) 
		{
			
			switch(i_nvic)
			{
				case 0:
					ADC_ConvertedValue_Ave[i_nvic++]= ADC_GetConversionValue(ADC1);
					break;
				case 1:
					ADC_ConvertedValue_Ave[i_nvic++]= ADC_GetConversionValue(ADC1);
					break;
				case 2:
					ADC_ConvertedValue_Ave[i_nvic++]= ADC_GetConversionValue(ADC1);
					break;
			}
			if(i_nvic==3)
			{
				for(i=0;i<3;i++)
				{
					ADC_ConvertedValueNVIC[i_avg]+=ADC_ConvertedValue_Ave[i];
				}
				ADC_ConvertedValueNVIC[i_avg++]=ADC_ConvertedValueNVIC[i_avg]/3;
				i_nvic=0;
				if(i_avg>2)
				{
					
					//22阈值目的为为30mv判断,并间隔2个值进行判断.
					if(ADC_ConvertedValueNVIC[i_avg-3]-ADC_ConvertedValueNVIC[i_avg-1]>500)
					{
						BAT1_SW2_OFF;
						//i_avg=FRE_NUM;
						kkk=1;
//						while(1)
//						{
//							if(kkk!=1)
//							{
//								kkk=1;
//							}
//							if(kkk==1)
//							{
//								break;
//							}
//						}
						ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
						ADC_SoftwareStartConvCmd(ADC1, DISABLE);
						ADC_DMACmd(ADC1, DISABLE);
						i_avg=0;
				
					}
				
				}
			}
			//LED1_TOGGLE;
			//LED1_ON;
			//BAT1_SW2_ON;
//			if((kkk==0)&&(i_nvic==3))
//			{
//				
//			}
//			if(i_avg==FRE_NUM)
//			{
//				ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
//				ADC_SoftwareStartConvCmd(ADC1, DISABLE);
//				ADC_DMACmd(ADC1, DISABLE);
//				i_avg=0;
//				
//				//k=0;
//			}
			
			
				
		}
		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
		
		
  
}

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
