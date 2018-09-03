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

extern void TimingDelay_Decrement(void);

extern __IO uint32_t flag;
extern  CanRxMsg RxMessage;
extern void TimingDelay_Decrement(void);

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[FRE_NUM];
__IO uint16_t ADC_ConvertedValue_Ave[FRE_NUM];
__IO uint32_t i=0;
__IO uint32_t i_nvic=0,i_avg=0,j=0,k=0;


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
					ADC_ConvertedValue[i_avg]+=ADC_ConvertedValue_Ave[i];
				}
				ADC_ConvertedValue[i_avg++]=ADC_ConvertedValue[i_avg]/3;
				i_nvic=0;
				
			}
			if(k==0)
			{
				if(i_avg>3)
				{
					//22阈值目的为为30mv判断,并间隔2个值进行判断.
					if(ADC_ConvertedValue[i_avg-4]-ADC_ConvertedValue[i_avg-1]>22)
					{
						//printf("\r\n 找到下降点 %d \r\n", ADC_ConvertedValue[i_nvic-1]);
						j=i_avg;
						if(j!=i_avg)
						{
							j=i_avg;
						}
						if(j==i_avg)
						{
							i_avg=FRE_NUM-3;
						}
						k=1;
				
					}
				
				}
			}
			if(i_avg==FRE_NUM)
			{
				printf("\r\n 找到下降点 %d \r\n", ADC_ConvertedValue[j-1]);
				ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
				
				for(i=0;i<FRE_NUM;i++)
				{
					if(ADC_ConvertedValue[i]!=0)
					{
						printf("\r\n  %d \r\n", ADC_ConvertedValue[i]);
					}
//					else
//					{
//						//break;
//					}
					
				}
				j=0;
				i_avg=0;
				k=0;
			}
			
			//printf("\r\n  %d \r\n", ADC_ConvertedValue[0]);			
		}
		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
		
		//printf("\r\n  %d \r\n", ADC_ConvertedValue[0]);
  
}

//void ADC1_2_IRQHandler(void)
//{
//	int i;
//	
//  if (ADC_GetITStatus(ADC1,ADC_IT_EOC)==SET) 
//		{
//			// 读取 ADC 的转换值
//			//ADC_ConvertedValue[0] = ADC_GetConversionValue(ADC1);
//			ADC_ConvertedValue[i_nvic++] = ADC_GetConversionValue(ADC1);
//			if(i_nvic>1)
//			{
//				if(ADC_ConvertedValue[i_nvic-2]-ADC_ConvertedValue[i_nvic-1]>18)
//				{
//					//printf("\r\n 找到下降点 %d \r\n", ADC_ConvertedValue[i_nvic-1]);
//					j=i_nvic;
//					if(j!=i_nvic)
//					{
//						j=i_nvic;
//					}
//					if(j==i_nvic)
//					{
//						i_nvic=FRE_NUM-3;
//					}
//					
//					
//					//ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
//				}
//				
//			}
//			if(i_nvic==FRE_NUM)
//			{
//				printf("\r\n 找到下降点 %d \r\n", ADC_ConvertedValue[j-1]);
//				ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
//				i_nvic=0;
//				j=0;
//				for(i=0;i<FRE_NUM;i++)
//				{
//					if(ADC_ConvertedValue[i]!=0)
//					{
//						printf("\r\n  %d \r\n", ADC_ConvertedValue[i]);
//					}
//					else
//					{
//						//break;
//					}
//					
//				}
//			}
//			
//			//printf("\r\n  %d \r\n", ADC_ConvertedValue[0]);			
//		}
//		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
//		
//		//printf("\r\n  %d \r\n", ADC_ConvertedValue[0]);
//  
//}
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
