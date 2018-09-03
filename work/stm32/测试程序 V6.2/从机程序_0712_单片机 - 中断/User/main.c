
// ADC ��ͨ���ɼ�

#include "stm32f10x.h"
#include "bsp_adc.h"
#include "bsp_sw.h"
#include "bsp_dac.h"
//#include "bsp_led.h"
#include "bsp_led1.h"
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "can.h"
#include "bsp_iwdg.h"

#define A 88              //1A��DAC���

//CAN������
__IO uint32_t flag = 0xff;		//���ڱ�־�Ƿ���յ����ݣ����жϺ����и�ֵ
__IO uint32_t flagx = 0;		//���ڱ�־�Ƿ���յ����ݣ����жϺ����и�ֵ
extern __IO uint32_t flag_nvic ;   //���ڱ�־�Ƿ���յ�ADC�ж���ɣ����жϺ����и�ֵ
 uint32_t i_nvic = 0;   //���ڱ�־�Ƿ���յ�ADC�ж���ɣ����жϺ����и�ֵ
CanTxMsg TxMessage;			    //���ͻ�����
CanRxMsg RxMessage; 			//���ջ�����

//ADC��⴫�����
extern __IO uint32_t BAT1_ADC_Val1 , BAT1_ADC_Val2 , BAT1_ADC_Val3 ,BAT1_ADC_Valt ,LsiFreq;
__IO uint32_t Bat1V1,Bat1V2,Bat1V3,Bat1Vt;   
int data1[8]={0,0,0,0,0,0,0,0};
extern __IO int flag1;

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[FRE_NUM];
// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal;  
// �����ʱ
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
	
int main(void)
{	
//	int U_dis=0;
	float BAT1_ADC_Valm=0, U1=0, U2=0;
	int T_dis=0 , U_dis=0;
	SysTick_Init();
	USARTx_Config();
	DAC_Mode_Init();
	ADCx_Init_L();
	BAT_SW_GPIO_Config();
	BSP_LED_Init();
	CAN_Config();
	IWDG_Init();
	LED1_ON;
	
	
	while(1)
	{

//		if(flag_nvic==1)
//		{
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[0]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[1]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[2]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[3]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[4]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[5]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[6]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[7]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[8]); 
//			printf("\r\n  %d \r\n", ADC_ConvertedValue[9]); 
//			flag_nvic=0xff;
//			
//		}
		//ADC_ConvertedValueLocal = (float) (ADC_ConvertedValue[0]) / 4095 *3.3;
		

		//printf("\r\n  %d \r\n", ADC_ConvertedValue[0]); 
//	  printf("\r\n The current AD value = %d \r\n", ADC_ConvertedValue[0]); 
//		printf("\r\n The current AD value = 0x%05X \r\n", ADC_ConvertedValue[0]); 
//		printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 

		Delay(0xffffe); 
		
//		U1=ADC_test_now();
//		Delay_us(15);
//		Delay_us(4);

//		U2=ADC_test_now();
//		
//		
		
	
	}
}
/*********************************************END OF FILE**********************/

