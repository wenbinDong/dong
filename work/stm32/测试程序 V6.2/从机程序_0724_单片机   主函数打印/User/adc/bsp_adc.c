#include "bsp_adc.h"
#include "bsp_sw.h"
#include "bsp_dac.h"
#include "bsp_SysTick.h"
//__IO uint16_t ADC_ConvertedValue;

__IO uint16_t ADC_ConvertedValue[FRE_NUM];

__IO uint32_t BAT1_ADC_Val1 , BAT1_ADC_Val2 , BAT1_ADC_Val3 , BAT1_ADC_Valt;
__IO int ntimes1 , ntimes2 , ntimes3;
__IO int flag1=0;


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[FRE_NUM];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
__IO uint32_t ADC_ConvertedValueLocal1[FRE_NUM]; 
__IO uint32_t ADC_ConvertedValueLocal2[FRE_NUM]; 
__IO uint32_t ADC_ConvertedValueLocal3[FRE_NUM];
__IO uint32_t ADC_ConvertedValueLocalt[FRE_NUM];
__IO uint32_t ADC_ConvertedValueLocalt2[FRE_NUM];
/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  **/
void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	macADC_GPIO_APBxClock_FUN ( macADC_CLK | macADC_GPIO_CLK, ENABLE);
	
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
static void ADCx_Mode_Config1()
{
	
	ADC_InitTypeDef ADC_InitStructure;
	
	// ��ADCʱ��
	macADC_APBxClock_FUN ( macADC_CLK, ENABLE );
	
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
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ���ɺ궨���ʱ������
	ADC_RegularChannelConfig(macADCx, macADC_CHANNEL, 1, macADC_SAMPLETIME_H);
	
	// ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ
  ADC_ITConfig(macADCx, ADC_IT_EOC, ENABLE);
	
	
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
  * @brief  ADC�ж�����
  * @param  ��
  * @retval ��
  */
static void ADC_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*�ж�����*/
	  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;	   //ADC1�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //�����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
//L:���٣�buffer:FRE_NUM       8��Ƶ13.5����
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
//	for (i = 0; i<FRE_NUM; i++)
//	{
//		value1 += ADC_ConvertedValueLocal1[i];
//	}

//		BAT1_ADC_Val1=(uint32_t)((float)value1/ntimes1);
}

//�жϰ汾
float ADC_test_now()
{
	//int i = 0;
	//float dU,U_now;
	uint32_t value1 = 0;
	//ntimes1 = FRE_NUM_TEST;

	
	//����ſ���ʱ�򣬵�һ�ε�ѹ���ԣ���طŵ�ǰ��ѹ��
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
//	//����ſ���ʱ�򣬵�һ�ε�ѹ���ԣ���طŵ�ǰ��ѹ��
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
	
	/*dac-5A�ŵ�*/
	DAC_Output(REF_DAC);
	
	/*��ʱ150us�󣬵�طŵ�5A�ĵ�ѹ�ȶ�*/
	Delay_us(T_test1-2);  
	
	/*��ʱ��������1�������أ��ɼ�5A�ŵ��ѹ*/
	BAT1_SW1_ON;
	Delay_us(4);
	
	
	/*dac-10�ŵ�*/
	DAC_Output(REF_DAC1);
	
	/*��ʱ100us�󣬵�طŵ�10A�ĵ�ѹ�ȶ�*/
	Delay_us(150-2);
	
	/*��ʱ��������2�������أ��ɼ�10A�ŵ��ѹ*/
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
	int flag2=0;
	int time=0;
	uint32_t value1=0 ,value2=0, value3=0  ;
	float BAT1_ADC_Valm=0, U1=0, U2=0;
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
	
	/*��ʱ��������1�������أ��ɼ���ص�ѹ*/
	BAT1_SW1_ON;
	Delay_us(1);
	
	
	/*��طŵ����뿪�ؿ���*/
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


	
	/*��طŵ翪�عرգ��ŵ���ѹͨ��SW2���ر����ڵ����ϣ��رշŵ翪�ضԼ����Ӱ��*/
	BAT1_SG_OFF;
	flag2=0;
	
	
	//ת��Ϊ�����ֵ�ѹ
	BAT1_SEL_ON;
	Delay_us(18);

	
	ADC_test(3,58);

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
	
	//���ڲ�������t���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{			 		 
		value3 += ADC_ConvertedValueLocalt[i];
	}
	BAT1_ADC_Valt=(uint32_t)((float)value2/ntimes2);
	 
	
	//��������3���д������
	 
	for (i = 0; i<FRE_NUM; i++)
	{ 			 
		value3 += ADC_ConvertedValueLocal3[i];
	}
	BAT1_ADC_Val3=(uint32_t)((float)value3/ntimes3);


}


/*********************************************END OF FILE**********************/
