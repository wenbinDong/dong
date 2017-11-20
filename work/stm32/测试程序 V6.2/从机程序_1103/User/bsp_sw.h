
/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __SW_H
#define	__SW_H

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/


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

/* -------------------- BATTERY DISCHARGE PIN（SG1-6）-------------------- */
#define  BSP_BAT_SG_RCC_APB2Periph              RCC_APB2Periph_GPIOE
#define  BSP_BAT1_SG_PIN                        GPIO_Pin_9
#define  BSP_BAT1_SG_GPIO                       GPIOE

/* ------------电池小电流放电电压采样开关-------------- */
#define  BSP_BAT_SW2_RCC_APB2Periph              RCC_APB2Periph_GPIOE
#define  BSP_BAT1_SW2_PIN                        GPIO_Pin_12
#define  BSP_BAT1_SW2_GPIO                       GPIOE

/* -------------selective switch for ADC,selector--------------- */
#define  BSP_BAT_SEL_RCC_APB2Periph              RCC_APB2Periph_GPIOE
#define  BSP_BAT1_SEL_PIN                        GPIO_Pin_15
#define  BSP_BAT1_SEL_GPIO                       GPIOE

/* ------------电池大电流放电电压采样开关-------------- */
#define  BSP_BAT_SW1_RCC_APB2Periph              RCC_APB2Periph_GPIOA
#define  BSP_BAT1_SW1_PIN                        GPIO_Pin_1
#define  BSP_BAT1_SW1_GPIO                       GPIOA



/* -------------------- BATTERY DISCHARGE PIN -------------------- */
#define BAT1_SG_ON GPIO_SetBits(BSP_BAT1_SG_GPIO,BSP_BAT1_SG_PIN)
#define BAT1_SG_OFF GPIO_ResetBits(BSP_BAT1_SG_GPIO,BSP_BAT1_SG_PIN)

/* -------------------- sample hold switch  -------------------- */
#define BAT1_SW2_ON GPIO_SetBits(BSP_BAT1_SW2_GPIO,BSP_BAT1_SW2_PIN)
#define BAT1_SW2_OFF GPIO_ResetBits(BSP_BAT1_SW2_GPIO,BSP_BAT1_SW2_PIN)

/* -------------selective switch for ADC,selector--------------- */
#define BAT1_SEL_ON GPIO_SetBits(BSP_BAT1_SEL_GPIO,BSP_BAT1_SEL_PIN)
#define BAT1_SEL_OFF GPIO_ResetBits(BSP_BAT1_SEL_GPIO,BSP_BAT1_SEL_PIN)


/* -------------------- sample hold switch  -------------------- */
#define BAT1_SW1_ON GPIO_SetBits(BSP_BAT1_SW1_GPIO,BSP_BAT1_SW1_PIN)
#define BAT1_SW1_OFF GPIO_ResetBits(BSP_BAT1_SW1_GPIO,BSP_BAT1_SW1_PIN)



/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void BAT_SW_GPIO_Config(void);
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif /* __ADC_H */                                                  /* End of module include. */                
