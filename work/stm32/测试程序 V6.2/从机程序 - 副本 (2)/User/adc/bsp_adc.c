#include "bsp_adc.h"
#include "bsp_sw.h"
#include "bsp_dac.h"
#include "bsp_SysTick.h"
//__IO uint16_t ADC_ConvertedValue;

__IO uint16_t ADC_ConvertedValue[FRE_NUM];

__IO uint32_t BAT1_ADC_Val1 , BAT1_ADC_Val2 , BAT1_ADC_Val3;
__IO int ntimes1 , ntimes2 , ntimes3;


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[FRE_NUM];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
__IO uint32_t ADC_ConvertedValueLocal1[FRE_NUM]; 
__IO uint32_t ADC_ConvertedValueLocal2[FRE_NUM]; 
__IO uint32_t ADC_ConvertedValueLocal3[FRE_NUM];

/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	macADC_GPIO_APBxClock_FUN ( macADC_GPIO_CLK, ENABLE );
	
	// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin = macADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// ��ʼ�� ADC IO
	GPIO_Init(macADC_PORT, &GPIO_InitStructure);				
}

/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	// ��DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// ��ADCʱ��
	macADC_APBxClock_FUN ( macADC_CLK, ENABLE );
	
	// ��λDMA������
	DMA_DeInit(macADC_DMA_CHANNEL);
	
	// ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( macADCx->DR ) );
	
	// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 
	DMA_InitStructure.DMA_BufferSize = FRE_NUM;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// �洢����ַ�̶�
	//DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(macADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// ʹ�� DMA ͨ��
	DMA_Cmd(macADC_DMA_CHANNEL , ENABLE);
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��1��
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// ��ʼ��ADC
	ADC_Init(macADCx, &ADC_InitStructure);
	
	// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ���ɺ궨���ʱ������
	ADC_RegularChannelConfig(macADCx, macADC_CHANNEL, 1, macADC_SAMPLETIME);
	
	
	// ʹ��ADC DMA ����
	ADC_DMACmd(macADCx, ENABLE);
	
	// ����ADC ������ʼת��
	ADC_Cmd(macADCx, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(macADCx);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(macADCx));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(macADCx);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(macADCx));
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(macADCx, ENABLE);
}

/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}


void ADC_test(uint8_t num,int time)
{
	int i = 0;
	
	ntimes1 = FRE_NUM;
	ntimes2 = FRE_NUM;
	ntimes3 = FRE_NUM;
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(macADCx, DISABLE);
	ADC_DMACmd(macADCx, DISABLE);
	
	/*�����ֵ����*/
	for (i = 0;i<FRE_NUM; i++)
	{
		ADC_ConvertedValue[i] = 0;	
	}
		
	ADC_SoftwareStartConvCmd(macADCx, ENABLE);
	ADC_DMACmd(macADCx, ENABLE);
	/*���ʱ����Ҫ����dma����Ĵ����й�*/
	Delay_us(time);
		
		
		//����ſ���ʱ�򣬵�һ�ε�ѹ���ԣ���طŵ�ǰ��ѹ��
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
}


void BAT1_ADC_U_Three(void)
{
	int i=0;
	uint32_t value1=0 ,value2=0 ,value3=0;
	BAT1_ADC_Val1 = 0;
	BAT1_ADC_Val2 = 0;
	BAT1_ADC_Val3 = 0;
		
	/*BAT1_SG_ON   ��طŵ翪��*/
	/*BAT1_SW1_ON  ���ݱ��ֵ�ش�����ŵ�ʱ��ѹ�Ŀ��أ�ADC�����������������*/
	/*BAT1_SW2_ON	 ���ݱ��ֵ��С�����ŵ�ʱ��ѹ�Ŀ���*/
	/*BAT1_SEL_ON	 ADC����л���ѹ���ߵ�ƽ��������ֵ�ѹ*/
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
	*2016.12.06����Ľ�
	***************************/

	 /*ADC��1�β�����������طŵ�ǰ��ѹ*/
	ADC_test(1,98);
	

	/*��طŵ����뿪�ؿ���*/
	BAT1_SG_ON;
	
	/*dac-5A�ŵ�*/
	DAC_Output(REF_DAC);
	
	/*��ʱ150us�󣬵�طŵ�5A�ĵ�ѹ�ȶ�*/
	Delay_us(250-2);
	
	/*��ʱ��������1�������أ��ɼ�5A�ŵ��ѹ*/
	BAT1_SW1_ON;
	Delay_us(4);
	
	
	/*dac-10A�ŵ�*/
	DAC_Output(REF_DAC1);
	
	/*��ʱ100us�󣬵�طŵ�10A�ĵ�ѹ�ȶ�*/
	Delay_us(150-2);
	
	/*��ʱ��������1�������أ��ɼ�10A�ŵ��ѹ*/
	BAT1_SW2_ON;
	Delay_us(4);
	
	/*��طŵ翪�عرգ��ŵ���ѹͨ��SW2���ر����ڵ����ϣ��رշŵ翪�ضԼ����Ӱ��*/
	BAT1_SG_OFF;
	
	/*ADC��2�μ�⣬���5A�ŵ�ʱ��ص�ѹ(�ŵ���ѹͨ��SW2���ر����ڵ�����)*/
	ADC_test(2,58);
	
	
	/*����ADC����л���ƽ��׼�����ֵ�ѹ*/
	BAT1_SEL_ON;
	/*�������غ�ADC���ŵ�ѹ�����������̣���Ҫ��ʱ20us�󣬵�ѹֵ�ȶ�*/
	Delay_us(18);
	
	/*ADC��3�μ�⣬����ش������С�����ŵ��Ĳ�ֵ�ѹ*/
	ADC_test(3,58);
	
	
	///////////////////////////////////////////

	/*ADC����л���ƽ���رտ���*/
	BAT1_SEL_OFF;
	/*������ŵ��ѹ�ɼ����أ��رտ���*/
	BAT1_SW2_OFF;
	/*С�����ŵ��ѹ�ɼ����أ��رտ���*/
	BAT1_SW1_OFF; 	 
	 
	//��������1���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{
		value1 += ADC_ConvertedValueLocal1[i];
	}
	
		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);
	 
	//��������2���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{			 		 
		value2 += ADC_ConvertedValueLocal2[i];
	}
	BAT1_ADC_Val2=(uint32_t)((float)value2/ntimes2);
	 
	
	//��������3���д������
	 
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
		
	/*BAT1_SG_ON   ��طŵ翪��*/
	/*BAT1_SW1_ON  ���ݱ��ֵ�ش�����ŵ�ʱ��ѹ�Ŀ��أ�ADC�����������������*/
	/*BAT1_SW2_ON	 ���ݱ��ֵ��С�����ŵ�ʱ��ѹ�Ŀ���*/
	/*BAT1_SEL_ON	 ADC����л���ѹ���ߵ�ƽ��������ֵ�ѹ*/
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
	*2016.12.14����Ľ�
	***************************/

	 /*ADC��1�β�����������طŵ�ǰ��ѹ*/
	ADC_test(1,98);
	

	/*��طŵ����뿪�ؿ���*/
	BAT1_SG_ON;
	
	/*dac-8.5A�ŵ�*/
	DAC_Output(REF_DAC);
	
	/*��ʱ150us�󣬵�طŵ�5A�ĵ�ѹ�ȶ�*/
	Delay_us(T_test1-2);  
	
	/*��ʱ��������1�������أ��ɼ�5A�ŵ��ѹ*/
	BAT1_SW1_ON;
	Delay_us(4);
	
	
	/*dac-9.5A�ŵ�*/
	DAC_Output(REF_DAC1);
	
	/*��ʱ100us�󣬵�طŵ�10A�ĵ�ѹ�ȶ�*/
	Delay_us(150-2);
	
	/*��ʱ��������1�������أ��ɼ�10A�ŵ��ѹ*/
	BAT1_SW2_ON;
	Delay_us(4);
	
	/*��طŵ翪�عرգ��ŵ���ѹͨ��SW2���ر����ڵ����ϣ��رշŵ翪�ضԼ����Ӱ��*/
	BAT1_SG_OFF;
	
	/*ADC��2�μ�⣬���5A�ŵ�ʱ��ص�ѹ(�ŵ���ѹͨ��SW2���ر����ڵ�����)*/
	ADC_test(2,58);
	
	
	/*����ADC����л���ƽ��׼�����ֵ�ѹ*/
	BAT1_SEL_ON;
	/*�������غ�ADC���ŵ�ѹ�����������̣���Ҫ��ʱ20us�󣬵�ѹֵ�ȶ�*/
	Delay_us(18);
	
	/*ADC��3�μ�⣬����ش������С�����ŵ��Ĳ�ֵ�ѹ*/
	ADC_test(3,58);
	
	
	///////////////////////////////////////////

	/*ADC����л���ƽ���رտ���*/
	BAT1_SEL_OFF;
	/*������ŵ��ѹ�ɼ����أ��رտ���*/
	BAT1_SW2_OFF;
	/*С�����ŵ��ѹ�ɼ����أ��رտ���*/
	BAT1_SW1_OFF; 	 
	 
	//��������1���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{
		value1 += ADC_ConvertedValueLocal1[i];
	}
	
		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);
	 
	//��������2���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{			 		 
		value2 += ADC_ConvertedValueLocal2[i];
	}
	BAT1_ADC_Val2=(uint32_t)((float)value2/ntimes2);
	 
	
	//��������3���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{ 			 
		value3 += ADC_ConvertedValueLocal3[i];
	}
	BAT1_ADC_Val3=(uint32_t)((float)value3/ntimes3);
}

void BAT1_ADC_U_Two(int U_dis)
{
	int i=0;
	uint32_t value1=0 ,value2=0;
	BAT1_ADC_Val1 = 0;
	BAT1_ADC_Val2 = 0;
		
	/*BAT1_SG_ON   ��طŵ翪��*/
	/*BAT1_SW1_ON  ���ݱ��ֵ�ش�����ŵ�ʱ��ѹ�Ŀ��أ�ADC�����������������*/
	/*BAT1_SW2_ON	 ���ݱ��ֵ��С�����ŵ�ʱ��ѹ�Ŀ���*/
	/*BAT1_SEL_ON	 ADC����л���ѹ���ߵ�ƽ��������ֵ�ѹ*/
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
	*2016.12.14����Ľ�
	***************************/

	 /*ADC��1�β�����������طŵ�ǰ��ѹ*/
	ADC_test(1,98);
	


	
//	/*dac-8.5A�ŵ�*/
//	DAC_Output(REF_DAC);
//	
//	/*��ʱ150us�󣬵�طŵ�5A�ĵ�ѹ�ȶ�*/
//	Delay_us(120-2);
	
	/*��ʱ��������1�������أ��ɼ���ص�ѹ*/
	BAT1_SW1_ON;
	Delay_us(4);
	
	
	/*��طŵ����뿪�ؿ���*/
	BAT1_SG_ON;
	/*dac-10A�ŵ�*/
	DAC_Output(U_dis);
	
	/*��ʱ100us�󣬵�طŵ��ѹ�ȶ�*/
	Delay_us(250-2);
	
	/*��ʱ��������2�������أ��ɼ��ŵ�ʱ��ѹ*/
	BAT1_SW2_ON;
	Delay_us(4);
	
	/*��طŵ翪�عرգ��ŵ���ѹͨ��SW2���ر����ڵ����ϣ��رշŵ翪�ضԼ����Ӱ��*/
	BAT1_SG_OFF;
	
//	/*ADC��2�μ�⣬���5A�ŵ�ʱ��ص�ѹ(�ŵ���ѹͨ��SW2���ر����ڵ�����)*/
//	ADC_test(2,58);
	
	
	/*����ADC����л���ƽ��׼�����ֵ�ѹ*/
	BAT1_SEL_ON;
	/*�������غ�ADC���ŵ�ѹ�����������̣���Ҫ��ʱ20us�󣬵�ѹֵ�ȶ�*/
	Delay_us(18);
	
	/*ADC��3�μ�⣬����ش������С�����ŵ��Ĳ�ֵ�ѹ*/
	ADC_test(2,58);
	
	
	///////////////////////////////////////////

	/*ADC����л���ƽ���رտ���*/
	BAT1_SEL_OFF;
	/*������ŵ��ѹ�ɼ����أ��رտ���*/
	BAT1_SW2_OFF;
	/*С�����ŵ��ѹ�ɼ����أ��رտ���*/
	BAT1_SW1_OFF; 	 
	 
	//��������1���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{
		value1 += ADC_ConvertedValueLocal1[i];
	}
	
		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);
	 
	//��������2���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{			 		 
		value2 += ADC_ConvertedValueLocal2[i];
	}
	BAT1_ADC_Val2=(uint32_t)((float)value2/ntimes2);
	 
	
	//��������3���д������
	 
//	for (i = 0; i<FRE_NUM; i++)
//	{ 			 
//		value3 += ADC_ConvertedValueLocal3[i];
//	}
//	BAT1_ADC_Val3=(uint32_t)((float)value3/ntimes3);
}
/*********************************************END OF FILE**********************/
