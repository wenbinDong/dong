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
#define A 88              //1A的DAC输出

volatile u32 time = 0; // ms 计时变量 

__IO uint32_t flag = 0xff;		//用于标志是否接收到数据，在中断函数中赋值
__IO uint32_t usartflag = 0xff;		//用于标志是否接收到数据，在中断函数中赋值
__IO uint32_t res1,res2,res3;
CanTxMsg TxMessage;			    //发送缓冲区
CanRxMsg RxMessage; 			//接收缓冲区


int main(void)
{
	float REF_VOL=3000 , I; 
	float U1 = 0 , U2 = 0 , U3 = 0;
	int data[4]={0} , i=0 , k=0;

	int time_0 = 0 , judge = 0;
	int cir_sec = 1;       		//两次测量间隔时间/s
	int cir_sec_2 = 30;
	int time_max=200;         //秒

	int U_dis = 1*A;        //199    880 10A
	int U_dis_d = A/8;
	int ntimes  = 1;
	int U_dis_max = 10*A;
	
	int U_dis_list[6]={5*A , 10*A , 7*A , 8*A , 6*A , 9*A};

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
	printf( "\r\n    i       t/s            U0            U1           dU        I \r\n");
	printf( "\r\n    0         0             0             0            0        0 \r\n");
	time = cir_sec_2 * 1000 - 500;
	
	
	
	
	while(1)  //time_0<time_max
	{
		if ( time == cir_sec_2 * 1000 ) // 1000 * 1 ms = 1s 时间到
		{
			k = 0;
			time = 0;
			
			while(k<=5)  //time_0<time_max
			{
				if ( time == cir_sec * 1000 ) // 1000 * 1 ms = 1s 时间到      
				{
					time = 0;
					
					/* LED1 取反 */      
					LED1_TOGGLE;
					
					U_dis = U_dis_list[k];
					data[0]=U_dis % 100;
					data[1]=U_dis / 100;
					
					CAN_SetData_n(4,0x1003,data);
					CAN_Transmit(CAN1, &TxMessage);
					
					LED_Delay(0xFFFF);
					time_0=time_0+cir_sec;    //time_0=time_0+cir_sec
					
					if( flag != 0xff )
					{
						
						i++;
						
						U1 = (float) (0x100 * RxMessage.Data[1] + RxMessage.Data[0]) / 4095 * REF_VOL;     //电池电压		
		//				U2 = (float) (0x100 * RxMessage.Data[3] + RxMessage.Data[2]) / 4095 * REF_VOL;     //半差电压
						U3 = (float) (0x100 * RxMessage.Data[5] + RxMessage.Data[4]) / 4095 * REF_VOL;     //差分电压
						U1 = U1*1.5;
						U3 = U3*1.5/8.5; 
						U2 = U1-U3;
						I = (float)U_dis/88;
						printf("%5d  %8d   %11.3f   %11.3f   %10.3f   %6.2f\r\n", i , time_0 , U1 , U2 , U3 , I);
						
						flag = 0xff;
						
					}
					if ( time_0>=ntimes*cir_sec*(judge+1))         //测量次数函数，judge+1即跳转到下一电流值
					{
						judge++;
						k++;
					}
				}
			}
			printf( "\r\n    0         0             0             0            0        0 \r\n");
			time_0 = time_0 + cir_sec_2;
		}
	}
}
/*********************************************END OF FILE**********************/
