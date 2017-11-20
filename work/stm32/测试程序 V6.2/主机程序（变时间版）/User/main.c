/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO-MINI STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "stm32f10x.h"
#include "math.h"
#include "bsp_usart1.h"
#include "bsp_TiMbase.h"
#include "bsp_led.h"
#include "can.h"


volatile u32 time = 0; // ms 计时变量 

__IO uint32_t flag = 0xff;		//用于标志是否接收到数据，在中断函数中赋值
__IO uint32_t usartflag = 0xff;		//用于标志是否接收到数据，在中断函数中赋值
__IO uint32_t res1,res2,res3;
CanTxMsg TxMessage;			    //发送缓冲区
CanRxMsg RxMessage; 			//接收缓冲区


int main(void)
{
	float REF_VOL=3000; 
	float U1 = 0 , U2 = 0 , U3 = 0;
	int data[4]={0} , i=0;
	int time_0=0 , judge=0;
	int cir_sec=2000;       		//两次测量间隔时间/ms
	int time_max=200;           //秒
	int T_test1=500;
	int T_test1_d=10;
	int ntimes  = 1;
	int T_test1_max = 500;


	USART1_Config();
	NVIC_Configuration();
	CAN_Config();
	LED_GPIO_Config();
	
	/* TIM2 定时配置 */	
  TIM2_Configuration();
	
	/* 实战定时器的中断优先级 */
	TIM2_NVIC_Configuration();
	
	/* TIM2 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	printf( "---------------------这是 “主机端” 的控制信息-------------------\r\n");
	printf( "\r\n    i       t/s             U1             U2             U3   T_test1 \r\n");
	
	time = 0*1000;
	
	
	
	
	while(T_test1<=T_test1_max)  //time_0<time_max
	{
		if ( time == cir_sec ) // 1000 * 1 ms = 1s 时间到      
    {
			
			time = 0;
			
			/* LED1 取反 */      
			LED1_TOGGLE;
			data[2]=T_test1 % 100;
	    data[3]=T_test1 / 100;
			CAN_SetData_n(4,0x1002,data);
			CAN_Transmit(CAN1, &TxMessage);
			
			LED_Delay(0xFFFF);
			time_0=time_0+cir_sec/1000;    //time_0=time_0+cir_sec
			
			if( flag != 0xff )
			{
				
				i++;
				
				U1 = (float) (0x100 * RxMessage.Data[1] + RxMessage.Data[0]) / 4095 * REF_VOL;     //电池电压		
				U2 = (float) (0x100 * RxMessage.Data[3] + RxMessage.Data[2]) / 4095 * REF_VOL;     //半差电压
				U3 = (float) (0x100 * RxMessage.Data[5] + RxMessage.Data[4]) / 4095 * REF_VOL;     //差分电压
				U3 = U3/8.5;
				printf("%5d  %8d   %12.6f   %12.6f   %12.6f   %4d\r\n", i , time_0 , U1 , U2 , U3 , T_test1 );
				
				flag = 0xff;
				
			}
//			if ( time_0>=ntimes*cir_sec/1000*(judge+1))         //测量次数函数，judge+1即跳转到下一电流值
//			{
//				judge++;
//				T_test1=T_test1+T_test1_d;
//			}
		}
	}
	
	
}
/*********************************************END OF FILE**********************/
