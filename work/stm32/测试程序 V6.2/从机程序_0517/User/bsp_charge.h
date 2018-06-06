/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*
* Filename      : bsp_charge.h
* Version       : V1.00
* Programmer(s) : XU
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __BSP_CHARGE_H
#define	__BSP_CHARGE_H


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "stm32f10x.h"


/*
*********************************************************************************************************
*                                          GPIO PIN DEFINITIONS
*********************************************************************************************************
*/
/* -------------------- BATTERY MEASURE EN PIN-------------------- */
#define  BSP_MEASURE_EN_RCC_APB2Periph          RCC_APB2Periph_GPIOD
#define  BSP_MEASURE_EN_PIN                     GPIO_Pin_11
#define  BSP_MEASURE_EN_GPIO                    GPIOD

/* -------------------- BATTERY CHARGE PIN-------------------- */
#define  BSP_BAT_CHARGE_RCC_APB2Periph          RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA
#define	 BSP_BAT1_CHARGE_PIN 									  GPIO_Pin_8
#define  BSP_BAT2_CHARGE_PIN 									  GPIO_Pin_9
#define  BSP_BAT3_CHARGE_PIN 									  GPIO_Pin_8
#define  BSP_BAT4_CHARGE_PIN  									GPIO_Pin_9
#define  BSP_BAT5_CHARGE_PIN  									GPIO_Pin_10
#define  BSP_BAT6_CHARGE_PIN  									GPIO_Pin_11
#define  BSP_BAT1_CHARGE_GPIO                   GPIOC
#define  BSP_BAT2_CHARGE_GPIO                   GPIOC
#define  BSP_BAT3_CHARGE_GPIO                   GPIOA
#define  BSP_BAT4_CHARGE_GPIO                   GPIOA
#define  BSP_BAT5_CHARGE_GPIO                   GPIOA
#define  BSP_BAT6_CHARGE_GPIO                   GPIOA

/* -------------------- EN PIN -------------------- */
#define  MEASURE_EN_PIN_ON   GPIO_SetBits(BSP_MEASURE_EN_GPIO,BSP_MEASURE_EN_PIN)
#define  MEASURE_EN_PIN_OFF  GPIO_ResetBits(BSP_MEASURE_EN_GPIO,BSP_MEASURE_EN_PIN)

#define	 BSP_BAT1_CHARGE_PIN_ON			GPIO_SetBits(BSP_BAT1_CHARGE_GPIO,BSP_BAT1_CHARGE_PIN)
#define	 BSP_BAT1_CHARGE_PIN_OFF		GPIO_ResetBits(BSP_BAT1_CHARGE_GPIO,BSP_BAT1_CHARGE_PIN)
#define	 BSP_BAT2_CHARGE_PIN_ON			GPIO_SetBits(BSP_BAT2_CHARGE_GPIO,BSP_BAT2_CHARGE_PIN)
#define	 BSP_BAT2_CHARGE_PIN_OFF		GPIO_ResetBits(BSP_BAT2_CHARGE_GPIO,BSP_BAT2_CHARGE_PIN)
#define	 BSP_BAT3_CHARGE_PIN_ON			GPIO_SetBits(BSP_BAT3_CHARGE_GPIO,BSP_BAT3_CHARGE_PIN)
#define	 BSP_BAT3_CHARGE_PIN_OFF		GPIO_ResetBits(BSP_BAT3_CHARGE_GPIO,BSP_BAT3_CHARGE_PIN)
#define	 BSP_BAT4_CHARGE_PIN_ON			GPIO_SetBits(BSP_BAT4_CHARGE_GPIO,BSP_BAT4_CHARGE_PIN)
#define	 BSP_BAT4_CHARGE_PIN_OFF		GPIO_ResetBits(BSP_BAT4_CHARGE_GPIO,BSP_BAT4_CHARGE_PIN)
#define	 BSP_BAT5_CHARGE_PIN_ON			GPIO_SetBits(BSP_BAT5_CHARGE_GPIO,BSP_BAT5_CHARGE_PIN)
#define	 BSP_BAT5_CHARGE_PIN_OFF		GPIO_ResetBits(BSP_BAT5_CHARGE_GPIO,BSP_BAT5_CHARGE_PIN)
#define	 BSP_BAT6_CHARGE_PIN_ON			GPIO_SetBits(BSP_BAT6_CHARGE_GPIO,BSP_BAT6_CHARGE_PIN)
#define	 BSP_BAT6_CHARGE_PIN_OFF		GPIO_ResetBits(BSP_BAT6_CHARGE_GPIO,BSP_BAT6_CHARGE_PIN)

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_CHARGE_Init(void);
void  Charge_Delay(__IO uint32_t nCount);

void  BAT_Charge_0(void);
void  BAT_Charge_1(void);
void  BAT_Charge_2(void);
void  BAT_Charge_3(void);
void  BAT_Charge_4(void);
void  BAT_Charge_5(void);
void  BAT_Charge_6(void);

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif
