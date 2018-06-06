#include "bsp_adc.h"
#include "bsp_sw.h"
#include "bsp_dac.h"
#include "bsp_SysTick.h"
//__IO uint16_t ADC_ConvertedValue;

__IO uint16_t ADC_ConvertedValue[FRE_NUM];

__IO uint32_t BAT1_ADC_Val1 , BAT1_ADC_Val2 , BAT1_ADC_Val3 , BAT1_ADC_Valt;
__IO int ntimes1 , ntimes2 , ntimes3;
__IO int flag1=0;


// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[FRE_NUM];

// 局部变量，用于保存转换计算后的电压值 	 
__IO uint32_t ADC_ConvertedValueLocal1[FRE_NUM]; 
__IO uint32_t ADC_ConvertedValueLocal2[FRE_NUM]; 
__IO uint32_t ADC_ConvertedValueLocal3[FRE_NUM];
__IO uint32_t ADC_ConvertedValueLocalt[FRE_NUM];
__IO uint32_t ADC_ConvertedValueLocalt2[FRE_NUM];

/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  **/
void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	macADC_GPIO_APBxClock_FUN ( macADC_GPIO_CLK, ENABLE );
	
	// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin = macADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// 初始化 ADC IO
	GPIO_Init(macADC_PORT, &GPIO_InitStructure);				
}

/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	// 打开DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// 打开ADC时钟
	macADC_APBxClock_FUN ( macADC_CLK, ENABLE );
	
	// 复位DMA控制器
	DMA_DeInit(macADC_DMA_CHANNEL);
	
	// 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( macADCx->DR ) );
	
	// 存储器地址，实际上就是一个内部SRAM的变量
  // DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// 数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 
	DMA_InitStructure.DMA_BufferSize = FRE_NUM;
	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// 存储器地址固定
	//DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	
	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// 初始化DMA
	DMA_Init(macADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// 使能 DMA 通道
	DMA_Cmd(macADC_DMA_CHANNEL , ENABLE);
	
	// ADC 模式配置
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道1个
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// 初始化ADC
	ADC_Init(macADCx, &ADC_InitStructure);
	
	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置 ADC 通道转换顺序为1，第一个转换，采样时间由宏定义的时钟周期
	ADC_RegularChannelConfig(macADCx, macADC_CHANNEL, 1, macADC_SAMPLETIME);
	
	
	// 使能ADC DMA 请求
	ADC_DMACmd(macADCx, ENABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(macADCx, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(macADCx);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(macADCx));
	
	// ADC开始校准
	ADC_StartCalibration(macADCx);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(macADCx));
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(macADCx, ENABLE);
}

//static void ADCx_Mode_Config2(void)
//{
//	DMA_InitTypeDef DMA_InitStructure;
//	ADC_InitTypeDef ADC_InitStructure;
//	
//	// 打开DMA时钟
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//	// 打开ADC时钟
//	macADC_APBxClock_FUN ( macADC_CLK, ENABLE );
//	
//	// 复位DMA控制器
//	DMA_DeInit(macADC_DMA_CHANNEL);
//	
//	// 配置 DMA 初始化结构体
//	// 外设基址为：ADC 数据寄存器地址
//	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( macADCx->DR ) );
//	
//	// 存储器地址，实际上就是一个内部SRAM的变量
//  // DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
//	
//	// 数据源来自外设
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//	
//	// 
//	DMA_InitStructure.DMA_BufferSize = FRE_NUM;
//	
//	// 外设寄存器只有一个，地址不用递增
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

//	// 存储器地址固定
//	//DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
//	
//	
//	// 外设数据大小为半字，即两个字节
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	
//	// 内存数据大小也为半字，跟外设数据大小相同
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	
//	// 循环传输模式
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

//	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	
//	// 禁止存储器到存储器模式，因为是从外设到存储器
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	
//	// 初始化DMA
//	DMA_Init(macADC_DMA_CHANNEL, &DMA_InitStructure);
//	
//	// 使能 DMA 通道
//	DMA_Cmd(macADC_DMA_CHANNEL , ENABLE);
//	
//	// ADC 模式配置
//	// 只使用一个ADC，属于单模式
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
//	
//	// 禁止扫描模式，多通道才要，单通道不需要
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

//	// 连续转换模式
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

//	// 不用外部触发转换，软件开启即可
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

//	// 转换结果右对齐
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//	
//	// 转换通道1个
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	
//		
//	// 初始化ADC
//	ADC_Init(macADCx, &ADC_InitStructure);
//	
//	// 配置ADC时钟Ｎ狿CLK2的4分频，即18MHz
//	RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
//	
//	// 配置 ADC 通道转换顺序为1，第一个转换，采样时间由宏定义的时钟周期
//	ADC_RegularChannelConfig(macADCx, macADC_CHANNEL, 1, macADC_SAMPLETIME);
//	
//	
//	// 使能ADC DMA 请求
//	ADC_DMACmd(macADCx, ENABLE);
//	
//	// 开启ADC ，并开始转换
//	ADC_Cmd(macADCx, ENABLE);
//	
//	// 初始化ADC 校准寄存器  
//	ADC_ResetCalibration(macADCx);
//	// 等待校准寄存器初始化完成
//	while(ADC_GetResetCalibrationStatus(macADCx));
//	
//	// ADC开始校准
//	ADC_StartCalibration(macADCx);
//	// 等待校准完成
//	while(ADC_GetCalibrationStatus(macADCx));
//	
//	// 由于没有采用外部触发，所以使用软件触发ADC转换 
//	ADC_SoftwareStartConvCmd(macADCx, ENABLE);
//}

/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}

//void ADCx_Init2(void)
//{
//	ADCx_GPIO_Config();
//	ADCx_Mode_Config2();
//}
void ADC_test(uint8_t num,int time)
{
	int i = 0;
	uint32_t value1 = 0;
	ntimes1 = FRE_NUM;
	ntimes2 = FRE_NUM;
	ntimes3 = FRE_NUM;
	
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(macADCx, DISABLE);
	ADC_DMACmd(macADCx, DISABLE);
	
	/*清空数值内容*/
	for (i = 0;i<FRE_NUM; i++)
	{
		ADC_ConvertedValue[i] = 0;	
	}
		
	ADC_SoftwareStartConvCmd(macADCx, ENABLE);
	ADC_DMACmd(macADCx, ENABLE);
	/*这个时间需要根据dma传输的次数有关*/
	Delay_us(time);
	
	//这里放控制时序，第一次电压测试（电池放电前电压）
	for (i = 0;i<FRE_NUM; i++)
	{
		if(ADC_ConvertedValue[i] == 0)
		{
			if(num == 1)
			{
				ntimes1--;
			}
			else if(num ==2)
			{
				ntimes2--;
			}
			else if(num == 3)
			{
				ntimes3--;
			}
			else
			{
				;
			}
		}
		if(num == 1)
		{
			ADC_ConvertedValueLocal1[i] =ADC_ConvertedValue[i];	
		}
		else if(num == 2)
		{
			ADC_ConvertedValueLocal2[i] =ADC_ConvertedValue[i];	
		}
		else if(num == 3)
		{
			ADC_ConvertedValueLocal3[i] =ADC_ConvertedValue[i];	
		}
		else
		{
			;
		}
	}
//	for (i = 0; i<FRE_NUM; i++)
//	{
//		value1 += ADC_ConvertedValueLocal1[i];
//	}

//		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);
}

void ADC_test_num(uint8_t num,int time,int FRE_NUM_t)
{
	int i = 0;
	uint32_t value1 = 0;
	ntimes1 = FRE_NUM_t;
	ntimes2 = FRE_NUM_t;
	ntimes3 = FRE_NUM_t;
	
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(macADCx, DISABLE);
	ADC_DMACmd(macADCx, DISABLE);
	
	/*清空数值内容*/
	for (i = 0;i<FRE_NUM_t; i++)
	{
		ADC_ConvertedValue[i] = 0;	
	}
		
	ADC_SoftwareStartConvCmd(macADCx, ENABLE);
	ADC_DMACmd(macADCx, ENABLE);
	/*这个时间需要根据dma传输的次数有关*/
	Delay_us(time);
	
	//这里放控制时序，第一次电压测试（电池放电前电压）
	for (i = 0;i<FRE_NUM_t; i++)
	{
		if(ADC_ConvertedValue[i] == 0)
		{
			if(num == 1)
			{
				ntimes1--;
			}
			else if(num ==2)
			{
				ntimes2--;
			}
			else if(num == 3)
			{
				ntimes3--;
			}
			else
			{
				;
			}
		}
		if(num == 1)
		{
			ADC_ConvertedValueLocal1[i] =ADC_ConvertedValue[i];	
		}
		else if(num == 2)
		{
			ADC_ConvertedValueLocal2[i] =ADC_ConvertedValue[i];	
		}
		else if(num == 3)
		{
			ADC_ConvertedValueLocal3[i] =ADC_ConvertedValue[i];	
		}
		else
		{
			;
		}
	}
//	for (i = 0; i<FRE_NUM; i++)
//	{
//		value1 += ADC_ConvertedValueLocal1[i];
//	}

//		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);
}

float ADC_test_now()
{
	int i = 0;
	float dU,U_now;
	uint32_t value1 = 0;
	ntimes1 = 2;
	
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(macADCx, DISABLE);
	ADC_DMACmd(macADCx, DISABLE);
	
	/*清空数值内容*/
	for (i = 0;i<6; i++)
	{
		ADC_ConvertedValue[i] = 0;	
	}
		
	ADC_SoftwareStartConvCmd(macADCx, ENABLE);
	ADC_DMACmd(macADCx, ENABLE);
	/*这个时间需要根据dma传输的次数有关*/
	Delay_us(8);
	
	//这里放控制时序，第一次电压测试（电池放电前电压）
	for (i = 0;i<6; i++)
	{
		if(ADC_ConvertedValue[i] == 0)
		{
			ntimes1--;
		}
		ADC_ConvertedValueLocal1[i] =ADC_ConvertedValue[i];	
	}
	for (i = 0; i<6; i++)
	{
		value1 += ADC_ConvertedValueLocal1[i];
	}

	BAT1_ADC_Valt=(uint32_t)((float)value1/ntimes1);
	dU = (float) (BAT1_ADC_Valt) / 4095 * 3000;
	U_now = dU * 1.5;
	return U_now;
}


uint32_t ADC_test_return()
{
//		int i = 0;
//	
//	ntimes1 = FRE_NUM;
//	ntimes2 = FRE_NUM;
//	ntimes3 = FRE_NUM;
	int i = 0;
	int ntimes = FRE_NUM/5;
	uint32_t value = 0,value1 = 0,value2 = 0;
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(macADCx, DISABLE);
	ADC_DMACmd(macADCx, DISABLE);
	
	/*清空数值内容*/
	for (i = 0;i<(FRE_NUM/10); i++)
	{
		ADC_ConvertedValue[i] = 0;	
	}

	ADC_SoftwareStartConvCmd(macADCx, ENABLE);
	ADC_DMACmd(macADCx, ENABLE);
	
	/*这个时间需要根据dma传输的次数有关*/  //20次100us 现在2次10us
	Delay_us(20);
	
	//这里放控制时序，第一次电压测试（电池放电前电压）
	for (i = 0;i<(FRE_NUM/5); i++)
	{
		if(ADC_ConvertedValue[i] == 0)
		{
				ntimes--;
		}
		
		ADC_ConvertedValueLocalt[i] =ADC_ConvertedValue[i];
		
	}
	
		//对于数据3进行处理分析
	 
	for (i = 0; i<(FRE_NUM/10); i++)
	{
		value += ADC_ConvertedValueLocalt[i];
	}
	BAT1_ADC_Valt = (uint32_t)((float)value/ntimes);  //>>1  代表除以二， 这样处理执行效率高
	
	return BAT1_ADC_Valt;
	
}


void BAT1_ADC_U_Three(void)
{
	int i=0;
	uint32_t value1=0 ,value2=0 ,value3=0;
	BAT1_ADC_Val1 = 0;
	BAT1_ADC_Val2 = 0;
	BAT1_ADC_Val3 = 0;
		
	/*BAT1_SG_ON   电池放电开关*/
	/*BAT1_SW1_ON  电容保持电池大电流放电时电压的开关，ADC检测引脚与其相连接*/
	/*BAT1_SW2_ON	 电容保持电池小电流放电时电压的开关*/
	/*BAT1_SEL_ON	 ADC检测切换电压，高电平，测量差分电压*/
	/*Timing sequence analyze
		  ______________________________________________________________________
	t:				|50|80      |2|120            |2|   40 |20 |   80    | = 394us
	
												_____________________
	SG:	_________________|     5A 	| 	10A		|____________________________
					                          	    ___________________________
	SW2:____________________________________|										    	|________
	
															   ___________________________________
	SW1:__________________________|	      	                          |________
																						 
  	                                                    _____________
	SEL:_________________________________________________|	  	      |________
																			        			  	
	
	adc:----------| adc1 |--------------------|  adc2  |------|  adc3 |---------
	
	*/
	
	/*************************
	*2016.12.06程序改进
	***************************/

	 /*ADC第1次测量，测量电池放电前电压*/
	ADC_test(1,98);
	

	/*电池放电引入开关开启*/
	BAT1_SG_ON;
	
	/*dac-5A放电*/
	DAC_Output(REF_DAC);
	
	
	/*延时150us后，电池放电5A的电压稳定*/
	Delay_us(250-2);
	
	/*此时开启电容1采样开关，采集5A放电电压*/
	BAT1_SW1_ON;
	Delay_us(4);
	
	
	/*dac-10A放电*/
	DAC_Output(REF_DAC1);
	
	/*延时100us后，电池放电10A的电压稳定*/
	Delay_us(150-2);
	
	/*此时开启电容1采样开关，采集10A放电电压*/
	BAT1_SW2_ON;
	Delay_us(4);
	
	/*电池放电开关关闭，放电后电压通过SW2开关保存在电容上，关闭放电开关对检测无影响*/
	BAT1_SG_OFF;
	
	/*ADC第2次检测，检测5A放电时电池电压(放电后电压通过SW2开关保存在电容上)*/
	ADC_test(2,58);
	
	
	/*开启ADC检测切换电平，准备测差分电压*/
	BAT1_SEL_ON;
	/*开启开关后，ADC引脚电压存在上升过程，需要延时20us后，电压值稳定*/
	Delay_us(18);
	
	/*ADC第3次检测，检测电池大电流和小电流放电后的差分电压*/
	ADC_test(3,58);
	
	
	///////////////////////////////////////////

	/*ADC检测切换电平，关闭开关*/
	BAT1_SEL_OFF;
	/*大电流放电电压采集开关，关闭开关*/
	BAT1_SW2_OFF;
	/*小电流放电电压采集开关，关闭开关*/
	BAT1_SW1_OFF; 	 
	 
	//对于数据1进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{
		value1 += ADC_ConvertedValueLocal1[i];
	}
	
		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);
	 
	//对于数据2进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{			 		 
		value2 += ADC_ConvertedValueLocal2[i];
	}
	BAT1_ADC_Val2=(uint32_t)((float)value2/ntimes2);
	 
	
	//对于数据3进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{ 			 
		value3 += ADC_ConvertedValueLocal3[i];
	}
	BAT1_ADC_Val3=(uint32_t)((float)value3/ntimes3);
}

void BAT1_ADC_U_Three2(int T_test1)
{
	int i=0;
	uint32_t value1=0 ,value2=0 ,value3=0;
	BAT1_ADC_Val1 = 0;
	BAT1_ADC_Val2 = 0;
	BAT1_ADC_Val3 = 0;
		
	/*BAT1_SG_ON   电池放电开关*/
	/*BAT1_SW1_ON  电容保持电池大电流放电时电压的开关，ADC检测引脚与其相连接*/
	/*BAT1_SW2_ON	 电容保持电池小电流放电时电压的开关*/
	/*BAT1_SEL_ON	 ADC检测切换电压，高电平，测量差分电压*/
	/*Timing sequence analyze
		  ______________________________________________________________________
	t:				|50|80      |2|120            |2|   40 |20 |   80    | = 394us
	
												_____________________
	SG:	_________________|     5A 	| 	10A		|____________________________
					                          	    ___________________________
	SW2:____________________________________|										    	|________
	
															   ___________________________________
	SW1:__________________________|	      	                          |________
																						 
  	                                                    _____________
	SEL:_________________________________________________|	  	      |________
																			        			  	
	
	adc:----------| adc1 |--------------------|  adc2  |------|  adc3 |---------
	
	*/
	
	/*************************
	*2016.12.14程序改进
	***************************/

	 /*ADC第1次测量，测量电池放电前电压*/
	ADC_test(1,98);
	

	/*电池放电引入开关开启*/
	BAT1_SG_ON;
	
	/*dac-5A放电*/
	DAC_Output(REF_DAC);
	
	/*延时150us后，电池放电5A的电压稳定*/
	Delay_us(T_test1-2);  
	
	/*此时开启电容1采样开关，采集5A放电电压*/
	BAT1_SW1_ON;
	Delay_us(4);
	
	
	/*dac-10放电*/
	DAC_Output(REF_DAC1);
	
	/*延时100us后，电池放电10A的电压稳定*/
	Delay_us(150-2);
	
	/*此时开启电容2采样开关，采集10A放电电压*/
	BAT1_SW2_ON;
	Delay_us(4);
	
	/*电池放电开关关闭，放电后电压通过SW2开关保存在电容上，关闭放电开关对检测无影响*/
	BAT1_SG_OFF;
	
	/*ADC第2次检测，检测5A放电时电池电压(放电后电压通过SW2开关保存在电容上)*/
	ADC_test(2,58);
	
	
	/*开启ADC检测切换电平，准备测差分电压*/
	BAT1_SEL_ON;
	/*开启开关后，ADC引脚电压存在上升过程，需要延时20us后，电压值稳定*/
	Delay_us(18);
	
	/*ADC第3次检测，检测电池大电流和小电流放电后的差分电压*/
	ADC_test(3,58);
	
	
	///////////////////////////////////////////

	/*ADC检测切换电平，关闭开关*/
	BAT1_SEL_OFF;
	/*大电流放电电压采集开关，关闭开关*/
	BAT1_SW2_OFF;
	/*小电流放电电压采集开关，关闭开关*/
	BAT1_SW1_OFF; 	 
	 
	//对于数据1进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{
		value1 += ADC_ConvertedValueLocal1[i];
	}
	
		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);
	 
	//对于数据2进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{			 		 
		value2 += ADC_ConvertedValueLocal2[i];
	}
	BAT1_ADC_Val2=(uint32_t)((float)value2/ntimes2);
	 
	
	//对于数据3进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{ 			 
		value3 += ADC_ConvertedValueLocal3[i];
	}
	BAT1_ADC_Val3=(uint32_t)((float)value3/ntimes3);
}

void BAT1_ADC_U_Two(int U_dis)
{
	int i=0;
	int flag2=0;
	int time=0;
	int time_max=1000;
	uint32_t value1=0 ,value2=0, value3=0 , battery_du ;
	float BAT1_ADC_Valm=0;
	BAT1_ADC_Val1 = 0;
	BAT1_ADC_Val2 = 0;
		
	/*BAT1_SG_ON   电池放电开关*/
	/*BAT1_SW1_ON  电容保持电池大电流放电时电压的开关，ADC检测引脚与其相连接*/
	/*BAT1_SW2_ON	 电容保持电池小电流放电时电压的开关*/
	/*BAT1_SEL_ON	 ADC检测切换电压，高电平，测量差分电压*/
	/*Timing sequence analyze
		  ______________________________________________________________________
	t:				|50|80      |2|120            |2|   40 |20 |   80    | = 394us
	
												_____________________
	SG:	_________________|     10A           	|________________________
						    _____________________________________________
	SW1:__________|	                                          |________
					                        ___________________________																				 
  SW2:____________________________|								          |________
	                                                ___________
	SEL:____________________________________________|	  	    |________
	
	adc:-------------| adc1 |-----------------------|  adc2  |---------
	
	*/
	
	/*************************
	*2016.12.14程序改进
	***************************/
	
	
	/*ADC第1次测量，测量电池放电前电压*/
	ADC_test(1,98);
	
	/*此时开启电容1采样开关，采集电池电压*/
	BAT1_SW1_ON;
	Delay_us(1);time+=1;
	
	
	/*电池放电引入开关开启*/
	BAT1_SG_ON;
	
	//转换为输出差分电压
	BAT1_SEL_ON;
	Delay_us(18);time+=18;
	
	BAT1_SW2_ON;
	Delay_us(1);time+=1;
	
	Delay_us(5);time+=5;
	
	BAT1_ADC_Valm=ADC_test_now();
	time+=8;
	
	while(BAT1_ADC_Valm<20)
	{
		if((time > 50) && (flag2 == 0))
		{
			DAC_Output(U_dis);
			flag2=1;
		}
		BAT1_SW2_OFF;
		Delay_us(1);time+=1;
		
		Delay_us(20);time+=10;
		
		BAT1_SW2_ON;
		Delay_us(1);time+=1;
		
		Delay_us(5);time+=5;
		
		BAT1_ADC_Valm=ADC_test_now();
		time+=8;
		
	}

//	/*dac-   放电*/
//	DAC_Output(U_dis);
//	
//	Delay_us(5);time+=5;
//	
//	
//	//
//	BAT1_SW2_ON;
//	Delay_us(4);time+=4;
//	
//	//转换为输出差分电压
//	BAT1_SEL_ON;
//	Delay_us(18);time+=18;
//	
//	/*ADC第2次检测，检测电池大电流和小电流放电后的差分电压*/
//	ADC_test_num(2,10,3);
	
	//Delay_us(90);time+=70;
	
	BAT1_SW2_OFF;
	Delay_us(4);time+=4;
	
	Delay_us(100);time+=100;
	
		//
//	BAT1_SW2_ON;
//	Delay_us(4);time+=4;

//	
//	/*ADC第2次检测，检测电池大电流和小电流放电后的差分电压*/
//	ADC_test_num(2,15,5);
//	
//	//Delay_us(90);time+=70;
//	
//	BAT1_SW2_OFF;
//	Delay_us(4);time+=4;
//	
//	Delay_us(20);time+=70;
	
	//
	BAT1_SW2_ON;
	Delay_us(1);time+=1;

	
	/*电池放电开关关闭，放电后电压通过SW2开关保存在电容上，关闭放电开关对检测无影响*/
	BAT1_SG_OFF;
	flag2=0;
	
	//ADCx_Init();

//	//转换为输出差分电压
//	BAT1_SEL_ON;
//	Delay_us(18);time+=18;	
	
	ADC_test(3,58);

	/*ADC检测切换电平，关闭开关*/
	BAT1_SEL_OFF;
	/*大电流放电电压采集开关，关闭开关*/
	BAT1_SW2_OFF;
	/*小电流放电电压采集开关，关闭开关*/
	BAT1_SW1_OFF; 	 
	 
	//对于数据1进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{
		value1 += ADC_ConvertedValueLocal1[i];
	}

		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);

	//对于数据2进行处理分析

	for (i = 0; i<FRE_NUM; i++)
	{
		value2 += ADC_ConvertedValueLocal2[i];
	}
	BAT1_ADC_Val2=(uint32_t)((float)value2/ntimes2);
	
	//对于测试数据t进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{			 		 
		value3 += ADC_ConvertedValueLocalt[i];
	}
	BAT1_ADC_Valt=(uint32_t)((float)value2/ntimes2);
	 
	
	//对于数据3进行处理分析
	 
	for (i = 0; i<FRE_NUM; i++)
	{ 			 
		value3 += ADC_ConvertedValueLocal3[i];
	}
	BAT1_ADC_Val3=(uint32_t)((float)value3/ntimes3);


}

	
//	battery_du=ADC_test_return();
//	
//	while(battery_du<2)  //battery_du<2
//	{
//		if(time<=time_max)  //time<=time_max
//		{
//			//
//			BAT1_SW2_OFF;
//			Delay_us(4);time+=4;
//			
//			//
//			//BAT1_SEL_ON;
//			BAT1_SW2_ON;
//			Delay_us(4);time+=4;
//			battery_du=ADC_test_return();
//			
//		}
//		else
//		{
//			time=0;
//			printf("超时");
//			break;
//			
//		}
//	}
	
//	/*跳出循环后，延时固定时间，电池放电电压稳定*/
//	Delay_us(50-2);
//	

//	
////	/*延时100us后，电池放电电压稳定*/
//	//Delay_us(100-2);
//	BAT1_SW2_OFF;
//	Delay_us(4);time+=4;

//	
//	/*此时开启电容2采样开关，采集放电时电压*/
//	BAT1_SW2_ON;
//	Delay_us(4);
//	

//	
////	/*ADC第2次检测，检测5A放电时电池电压(放电后电压通过SW2开关保存在电容上)*/
////	ADC_test(2,58);
//	
//	
//	/*开启ADC检测切换电平，准备测差分电压*/
//	BAT1_SEL_ON;
//	/*开启开关后，ADC引脚电压存在上升过程，需要延时20us后，电压值稳定*/
//	Delay_us(18);
//	
//	/*ADC第3次检测，检测电池大电流和小电流放电后的差分电压*/
//	ADC_test(2,58);
	
	
	///////////////////////////////////////////
////////	/*延时100us后，电池放电电压稳定*/
////////	Delay_us(100-2);
////////	
////////	/*在这里添加对电压降的判断*/
//////////	LED1_ON;             //示波器测试函数时间
////////	dU_Current_0 = ADC_test_return();
//////////	LED1_OFF;
////////	while(flag1==0)
////////	{
//////////		(dU_Current_0 - dU_Current_1)
////////		Delay_us(10);
////////		dU_Current_1 = ADC_test_return();
////////		if((dU_Current_0 - dU_Current_1)>40)
////////		{
////////			flag1=1;
////////			break;
////////		}
////////		dU_Current_0 = dU_Current_1;

////////	}
/*********************************************END OF FILE**********************/
