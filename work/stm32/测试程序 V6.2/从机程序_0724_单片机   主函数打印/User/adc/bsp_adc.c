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
	macADC_GPIO_APBxClock_FUN ( macADC_CLK | macADC_GPIO_CLK, ENABLE);
	
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
static void ADCx_Mode_Config1()
{
	
	ADC_InitTypeDef ADC_InitStructure;
	
	// 打开ADC时钟
	macADC_APBxClock_FUN ( macADC_CLK, ENABLE );
	
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
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// 配置 ADC 通道转换顺序为1，第一个转换，采样时间由宏定义的时钟周期
	ADC_RegularChannelConfig(macADCx, macADC_CHANNEL, 1, macADC_SAMPLETIME_H);
	
	// ADC 转换结束产生中断，在中断服务程序中读取转换值
  ADC_ITConfig(macADCx, ADC_IT_EOC, ENABLE);
	
	
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

/**
  * @brief  ADC中断配置
  * @param  无
  * @retval 无
  */
static void ADC_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*中断设置*/
	  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;	   //ADC1中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
//L:慢速：buffer:FRE_NUM       8分频13.5采样
void ADCx_Init_L(void)
{
	ADC_NVIC_Config();
	ADCx_GPIO_Config();
	ADCx_Mode_Config1();
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

//中断版本
float ADC_test_now()
{
	//int i = 0;
	//float dU,U_now;
	uint32_t value1 = 0;
	//ntimes1 = FRE_NUM_TEST;

	
	//这里放控制时序，第一次电压测试（电池放电前电压）
//	for (i = 0;i<FRE_NUM_TEST; i++)
//	{
////		if(ADC_ConvertedValue[i] == 0)
////		{
////			ntimes1--;
////		}
//		value1 +=ADC_ConvertedValue[i];	
//	}
	value1 =ADC_ConvertedValue[0];	

//	BAT1_ADC_Valt=(uint32_t)((float)value1/ntimes1);
//	dU = (float) (BAT1_ADC_Valt) / 4095 * 3000;
//	U_now = dU * 1.5;
//	U_now = BAT1_ADC_Valt * 1.098;
  printf("%11.3f\r\n",value1);
	return value1;
	
}

//float ADC_test_now()
//{
//	int i = 0;
//	float dU,U_now;
//	uint32_t value1 = 0;
//	ntimes1 = FRE_NUM_TEST;
//	
//	
//	//这里放控制时序，第一次电压测试（电池放电前电压）
//	for (i = 0;i<FRE_NUM_TEST; i++)
//	{
//		if(ADC_ConvertedValue[i] == 0)
//		{
//			ntimes1--;
//		}
//		ADC_ConvertedValueLocal1[i] =ADC_ConvertedValue[i];	
//	}
//	for (i = 0; i<FRE_NUM_TEST; i++)
//	{
//		value1 += ADC_ConvertedValueLocal1[i];
//	}

//	//BAT1_ADC_Valt=(uint32_t)((float)value1/ntimes1);
//	dU = (float) (BAT1_ADC_Valt) / 4095 * 3000;
//	U_now = dU * 1.5;
//	printf("%11.3f\r\n",U_now);
//	return U_now;
//	
//}printf("%11.3f\r\n",U_now);




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
	uint32_t value1=0 ,value2=0, value3=0  ;
	float BAT1_ADC_Valm=0, U1=0, U2=0;
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
	Delay_us(1);
	
	
	/*电池放电引入开关开启*/
	BAT1_SG_ON;
	
	BAT1_SW2_ON;
	Delay_us(1);
	
	Delay_us(5);
	
	BAT1_SW2_OFF;
	Delay_us(1);
	U1=ADC_test_now();
	Delay_us(15);time+=10;
	BAT1_SW2_ON;
	Delay_us(4);

	U2=ADC_test_now();
	
	
//	while((U1-U2)<50)
//	{
//		U1=U2;
//		BAT1_SW2_OFF;
//		Delay_us(1);
//		
//		Delay_us(10);
//		
//		BAT1_SW2_ON;
//		Delay_us(1);
//		
//		U2=ADC_test_now();
//		if(flag2 == 0)
//		{
//			DAC_Output(U_dis);
//			flag2=1;
//		}
//	}

	DAC_Output(U_dis);
	BAT1_SW2_OFF;
	Delay_us(1);
	
	Delay_us(100);
	
		
	BAT1_SW2_ON;
	Delay_us(4);


	
	/*电池放电开关关闭，放电后电压通过SW2开关保存在电容上，关闭放电开关对检测无影响*/
	BAT1_SG_OFF;
	flag2=0;
	
	
	//转换为输出差分电压
	BAT1_SEL_ON;
	Delay_us(18);

	
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


/*********************************************END OF FILE**********************/
