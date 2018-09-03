/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO-MINI STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	
		GPIO_Init(GPIOD, &GPIO_InitStructure);	
		  
    
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOD, GPIO_Pin_6|GPIO_Pin_3);	 
}
/*********************************************END OF FILE**********************/
