/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �����жϽ��ղ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO-MINI STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "stm32f10x.h"
#include "math.h"
#include "bsp_usart1.h"
#include "bsp_TiMbase.h"
#include "bsp_led.h"
#include "can.h"


volatile u32 time = 0; // ms ��ʱ���� 

__IO uint32_t flag = 0xff;		//���ڱ�־�Ƿ���յ����ݣ����жϺ����и�ֵ
__IO uint32_t usartflag = 0xff;		//���ڱ�־�Ƿ���յ����ݣ����жϺ����и�ֵ
__IO uint32_t res1,res2,res3;
CanTxMsg TxMessage;			    //���ͻ�����
CanRxMsg RxMessage; 			//���ջ�����


int main(void)
{
	float REF_VOL=3000; 
	float U1 = 0 , U2 = 0 , U3 = 0;
	int data[4]={0} , i=0;
	int time_0=0;
	int cir_sec=1000;       		//���β������ʱ��/s
	int time_max=60 * 60 * 5;         //��
	

	USART1_Config();
	NVIC_Configuration();
	CAN_Config();
	LED_GPIO_Config();
	
	/* TIM2 ��ʱ���� */	
  TIM2_Configuration();
	
	/* ʵս��ʱ�����ж����ȼ� */
	TIM2_NVIC_Configuration();
	
	/* TIM2 ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	printf( "---------------------���� �������ˡ� �Ŀ�����Ϣ-------------------\r\n");
	printf( "\r\n    i       t/s             U1             U2             U3     \r\n");
	
	time = 0*1000; //��ʼ����ǰ�ĵȴ�ʱ��
	
	
	
	
	while(1)  //time_0<time_max
	{
		if ( time == cir_sec ) // 1000 * 1 ms = 1s ʱ�䵽      
    {
			
			time = 0;
			
			/* LED1 ȡ�� */      
			LED1_TOGGLE;
			
			CAN_SetData_n(4,0x1001,data);
			CAN_Transmit(CAN1, &TxMessage);
			
			LED_Delay(0xFFFF);
			time_0=time_0+cir_sec/1000;    //time_0=time_0+cir_sec
			
			if( flag != 0xff )
			{
				
				i++;
				
				U1 = (float) (0x100 * RxMessage.Data[1] + RxMessage.Data[0]) / 4095 * REF_VOL;     //��ص�ѹ		
				U2 = (float) (0x100 * RxMessage.Data[3] + RxMessage.Data[2]) / 4095 * REF_VOL;     //����ѹ
				U3 = (float) (0x100 * RxMessage.Data[5] + RxMessage.Data[4]) / 4095 * REF_VOL;     //��ֵ�ѹ
				U1 = U1*1.5;
				U2 = U2*1.5;
				U3 = U3*1.5/8.5; 
				printf("%5d  %8d   %12.3f   %12.3f   %12.3f  \r\n", i , time_0 , U1 , U2 , U3 );
				
				flag = 0xff;
				
			}
		}
	}
	while(1)
	{
		if ( time == 100 )
		{
			time = 0;
			LED1_TOGGLE;
		}
	}
}
/*********************************************END OF FILE**********************/
