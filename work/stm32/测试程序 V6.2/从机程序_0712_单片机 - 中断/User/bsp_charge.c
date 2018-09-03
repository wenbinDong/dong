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
* Filename      : bsp_charge.c
* Version       : V1.00
* Programmer(s) : XU
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_charge.h"


/*
*********************************************************************************************************
*                                          BSP_CHARGE_GPIO_Config()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : 
*
*********************************************************************************************************
*/

void  BSP_CHARGE_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	
	RCC_APB2PeriphClockCmd(BSP_MEASURE_EN_RCC_APB2Periph|BSP_BAT_CHARGE_RCC_APB2Periph, ENABLE); /*�������ŵ�����ʱ��*/
	/****************battery measure en pin****************/
	GPIO_InitStructure.GPIO_Pin =BSP_MEASURE_EN_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /*��������ģʽΪͨ���������*/ 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /*������������Ϊ50MHz */
	GPIO_Init(BSP_MEASURE_EN_GPIO, &GPIO_InitStructure);
	
	/****************battery charge pin****************/
		GPIO_InitStructure.GPIO_Pin = BSP_BAT1_CHARGE_PIN;	//BATTERY CHARGE PIN 1
	GPIO_Init(BSP_BAT1_CHARGE_GPIO, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = BSP_BAT2_CHARGE_PIN;	//BATTERY CHARGE PIN 2
	GPIO_Init(BSP_BAT2_CHARGE_GPIO, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = BSP_BAT3_CHARGE_PIN;	//BATTERY CHARGE PIN 3
	GPIO_Init(BSP_BAT3_CHARGE_GPIO, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = BSP_BAT4_CHARGE_PIN;	//BATTERY CHARGE PIN 4
	GPIO_Init(BSP_BAT4_CHARGE_GPIO, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = BSP_BAT5_CHARGE_PIN;	//BATTERY CHARGE PIN 5
	GPIO_Init(BSP_BAT5_CHARGE_GPIO, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = BSP_BAT6_CHARGE_PIN;	//BATTERY CHARGE PIN 6
	GPIO_Init(BSP_BAT6_CHARGE_GPIO, &GPIO_InitStructure);

	/* close all pin*/
	MEASURE_EN_PIN_OFF;
	BSP_BAT1_CHARGE_PIN_OFF;
	BSP_BAT2_CHARGE_PIN_OFF;
	BSP_BAT3_CHARGE_PIN_OFF;
	BSP_BAT4_CHARGE_PIN_OFF;
	BSP_BAT5_CHARGE_PIN_OFF;
	BSP_BAT6_CHARGE_PIN_OFF;
}


/*
*********************************************************************************************************
*                                         Charge_Delay(__IO uint32_t nCount)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : 
*
*********************************************************************************************************
*/

void Charge_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


/*
*********************************************************************************************************
*                                          BAT_Charge_0(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : 
*
*********************************************************************************************************
*/

void BAT_Charge_0(void)
{	
	MEASURE_EN_PIN_ON; //����ʹ���źţ���ʱ50us�������Ӳ������ֹ��������ջ���ЧӦ��
	
	Charge_Delay(0x000127);
	
	BSP_BAT1_CHARGE_PIN_OFF;
	BSP_BAT2_CHARGE_PIN_OFF;
	BSP_BAT3_CHARGE_PIN_OFF;
	BSP_BAT4_CHARGE_PIN_OFF;
	BSP_BAT5_CHARGE_PIN_OFF;
	BSP_BAT6_CHARGE_PIN_OFF;
	
	MEASURE_EN_PIN_OFF; //�ر�ʹ���ź�
}


/*
*********************************************************************************************************
*                                          BAT_Charge_1(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Only open balanced pin1	�ȹغ�
*
*********************************************************************************************************
*/

void BAT_Charge_1(void)
{
	MEASURE_EN_PIN_ON;	//����ʹ���źţ���ʱ50us�������Ӳ������ֹ��������ջ���ЧӦ��
	
	Charge_Delay(0x000127);
	
	BAT_Charge_0();
	
	MEASURE_EN_PIN_OFF;	//�ر�ʹ���ź�
	
	BSP_BAT1_CHARGE_PIN_ON;
}


/*
*********************************************************************************************************
*                                          BAT_Charge_2(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Only open balanced pin2	�ȹغ�
*
*********************************************************************************************************
*/

void BAT_Charge_2(void)
{
	MEASURE_EN_PIN_ON;	//����ʹ���źţ���ʱ50us�������Ӳ������ֹ��������ջ���ЧӦ��
	
	Charge_Delay(0x000127);
	
	BAT_Charge_0();
	
	MEASURE_EN_PIN_OFF;	//�ر�ʹ���ź�
	
	BSP_BAT2_CHARGE_PIN_ON;
}


/*
*********************************************************************************************************
*                                          BAT_Charge_3(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Only open balanced pin3	�ȹغ�
*
*********************************************************************************************************
*/

void BAT_Charge_3(void)
{
	MEASURE_EN_PIN_ON;	//����ʹ���źţ���ʱ50us�������Ӳ������ֹ��������ջ���ЧӦ��
	
	Charge_Delay(0x000127);
	
	BAT_Charge_0();
	
	MEASURE_EN_PIN_OFF;	//�ر�ʹ���ź�
	
	BSP_BAT3_CHARGE_PIN_ON;
}


/*
*********************************************************************************************************
*                                          BAT_Charge_4(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Only open balanced pin4	�ȹغ�
*
*********************************************************************************************************
*/

void BAT_Charge_4(void)
{
	MEASURE_EN_PIN_ON;	//����ʹ���źţ���ʱ50us�������Ӳ������ֹ��������ջ���ЧӦ��
	
	Charge_Delay(0x000127);
	
	BAT_Charge_0();
	
	MEASURE_EN_PIN_OFF;	//�ر�ʹ���ź�
	
	BSP_BAT4_CHARGE_PIN_ON;
}


/*
*********************************************************************************************************
*                                          BAT_Charge_5(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Only open balanced pin5	�ȹغ�
*
*********************************************************************************************************
*/

void BAT_Charge_5(void)
{
	MEASURE_EN_PIN_ON;	//����ʹ���źţ���ʱ50us�������Ӳ������ֹ��������ջ���ЧӦ��
	
	Charge_Delay(0x000127);
	
	BAT_Charge_0();
	
	MEASURE_EN_PIN_OFF;	//�ر�ʹ���ź�
	
	BSP_BAT5_CHARGE_PIN_ON;
}


/*
*********************************************************************************************************
*                                          BAT_Charge_6(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Only open balanced pin6	�ȹغ�
*
*********************************************************************************************************
*/

void BAT_Charge_6(void)
{
	MEASURE_EN_PIN_ON;	//����ʹ���źţ���ʱ50us�������Ӳ������ֹ��������ջ���ЧӦ��
	
	Charge_Delay(0x000127);
	
	BAT_Charge_0();
	
	MEASURE_EN_PIN_OFF;	//�ر�ʹ���ź�
	
	BSP_BAT6_CHARGE_PIN_ON;
}


/*
*********************************************************************************************************
*                                               BSP_CHARGE_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : 
*
*********************************************************************************************************
*/

void BSP_CHARGE_Init(void)
{
	BSP_CHARGE_GPIO_Config();
}


/*************************END OF FILE******************************/

