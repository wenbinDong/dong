/**
  ******************************************************************************
	* @attention
  *DAC_Output------PA4
  ******************************************************************************
  */ 
  
#include "bsp_dac.h"


/* @brief  使能DAC的时钟，初始化GPIO*/
static void DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

  /* 使能GPIOA时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	/* 使能DAC时钟 */	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
  /* DAC的GPIO配置，模拟输入 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* 配置DAC 通道1 */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;								//not by external trigger
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//不使用波形发生器
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//不使用DAC输出缓冲
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* 使能通道1 由PA4输出 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

/* @brief  DAC_Mode_Init */
void DAC_Mode_Init(void)
{
	DAC_Config();
	DAC_Output(0) ;
}

/* @brief  DAC_Output*/
//used it such as DAC_Output(1238) ;OR	DAC_Output(0) ;
void DAC_Output(u16 value)
{
//	float tempvalue = value ;
//	tempvalue = tempvalue / 1000 ;
//	tempvalue = tempvalue * 4095 / 3.3 ;
	DAC_SetChannel1Data(DAC_Align_12b_R,value) ;  
}

/*********************************************END OF FILE**********************/





























