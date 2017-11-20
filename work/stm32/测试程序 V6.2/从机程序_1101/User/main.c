
// ADC 单通道采集

#include "stm32f10x.h"
#include "bsp_adc.h"
#include "bsp_sw.h"
#include "bsp_dac.h"
//#include "bsp_led.h"
#include "bsp_led1.h"
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "can.h"

//CAN设置区
__IO uint32_t flag = 0xff;		//用于标志是否接收到数据，在中断函数中赋值
CanTxMsg TxMessage;			    //发送缓冲区
CanRxMsg RxMessage; 			//接收缓冲区

//ADC检测传输变量
extern __IO uint32_t BAT1_ADC_Val1 , BAT1_ADC_Val2 , BAT1_ADC_Val3 ,BAT1_ADC_Valt;
__IO uint32_t Bat1V1,Bat1V2,Bat1V3,Bat1Vt;   
int data1[8]={0,0,0,0,0,0,0,0};


// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[FRE_NUM];

// 软件延时
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
	
int main(void)
{	
//	int U_dis=0;
	int T_dis=0 , U_dis=0;
	SysTick_Init();
	USARTx_Config();
	DAC_Mode_Init();
	ADCx_Init();
	BAT_SW_GPIO_Config();
	BSP_LED_Init();
	CAN_Config();
	
	LED1_ON;
	
	
	while(1)
	{		
		while( flag == 0xff );
		
		switch(flag)
		{
			
			case 1:                  //ID:1001
				LED1_TOGGLE;

				BAT1_ADC_U_Three();
				
				Bat1V1 = BAT1_ADC_Val1;
				Bat1V2 = BAT1_ADC_Val2;
				Bat1V3 = BAT1_ADC_Val3;
			  
				data1[0] = Bat1V1%0x100;
				data1[1] = Bat1V1/0x100;
				data1[2] = Bat1V2%0x100;
				data1[3] = Bat1V2/0x100;
				data1[4] = Bat1V3%0x100;
				data1[5] = Bat1V3/0x100;
				data1[6] = 0;
				data1[7] = 0;
				
				CAN_SetData(0x41,data1);
				CAN_Transmit(CAN1, &TxMessage);		
				flag = 0xff;	
				Bat1V1 = 0;
				Bat1V2 = 0;
				Bat1V3 = 0;
			break;
				
			case 2:                  //ID:1002
				LED1_TOGGLE;
				
//				U_dis=100*RxMessage.Data[1]+RxMessage.Data[0];
				T_dis=100*RxMessage.Data[3]+RxMessage.Data[2];
				BAT1_ADC_U_Three2(T_dis);
				
				Bat1V1 = BAT1_ADC_Val1;
				Bat1V2 = BAT1_ADC_Val2;
				Bat1V3 = BAT1_ADC_Val3;
			
				data1[0] = Bat1V1%0x100;
				data1[1] = Bat1V1/0x100;
				data1[2] = Bat1V2%0x100;
				data1[3] = Bat1V2/0x100;
				data1[4] = Bat1V3%0x100;
				data1[5] = Bat1V3/0x100;
				data1[6] = 0;
				data1[7] = 0;
				
				CAN_SetData(0x41,data1);
				CAN_Transmit(CAN1, &TxMessage);		
				flag = 0xff;	
				Bat1V1 = 0;
				Bat1V2 = 0;
				Bat1V3 = 0;
			break;
			
			case 3:                  //ID:1003
				LED1_TOGGLE;
			
				U_dis=100*RxMessage.Data[1]+RxMessage.Data[0];
			
				BAT1_ADC_U_Two(U_dis);
				
				Bat1V1 = BAT1_ADC_Val1;
				Bat1V2 = BAT1_ADC_Val2;
				Bat1Vt = BAT1_ADC_Valt;
			
				data1[0] = Bat1V1%0x100;
				data1[1] = Bat1V1/0x100;
				data1[2] = Bat1Vt%0x100;
				data1[3] = Bat1Vt/0x100;
				data1[4] = Bat1V2%0x100;
				data1[5] = Bat1V2/0x100;
				data1[6] = 0;
				data1[7] = 0;
				
				CAN_SetData(0x41,data1);
				CAN_Transmit(CAN1, &TxMessage);		
				flag = 0xff;	
				Bat1V1 = 0;
				Bat1V2 = 0;
				Bat1V3 = 0;
			break;
			
			default:
				flag = 0xff;
			break;
		}
		
	}
}
/*********************************************END OF FILE**********************/

