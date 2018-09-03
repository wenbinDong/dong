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
* Filename      : bsp_led.h
* Version       : V1.00
* Programmer(s) : WEN
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

#ifndef __BSP_LED_H
#define	__BSP_LED_H


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

#include  "stm32f10x.h"


/*
*********************************************************************************************************
*                                          GPIO PIN DEFINITIONS
*********************************************************************************************************
*/

                          /* -------------------- GPIO PINS -------------------- */
//#define  BSP_LED_PIN1                         GPIO_Pin_5
#define  BSP_LED_PIN1                         GPIO_Pin_1
#define  BSP_LED_PIN2                         GPIO_Pin_6
#define  BSP_LED_PIN3                         GPIO_Pin_3


//#define  BSP_LED_PIN1                         GPIO_Pin_4
//#define  BSP_LED_PIN2                         GPIO_Pin_12
//#define  BSP_LED_PIN3                         GPIO_Pin_3
//#define  BSP_LED_PIN4                         GPIO_Pin_3

//#define  BSP_LED_GPIO1                        GPIOC
//#define  BSP_LED_GPIO2                        GPIOB
//#define  BSP_LED_GPIO3                        GPIOD
//#define  BSP_LED_GPIO4                        GPIOD

//#define  BSP_LED_GPIO1                        GPIOB
#define  BSP_LED_GPIO1                        GPIOC
#define  BSP_LED_GPIO2                        GPIOD
#define  BSP_LED_GPIO3                        GPIOD

//#define  BSP_LED_RCC_APB2Periph1              RCC_APB2Periph_GPIOC               
//#define  BSP_LED_RCC_APB2Periph2              RCC_APB2Periph_GPIOB 
//#define  BSP_LED_RCC_APB2Periph3              RCC_APB2Periph_GPIOD 
//#define  BSP_LED_RCC_APB2Periph4              RCC_APB2Periph_GPIOD

//#define  BSP_LED_RCC_APB2Periph1              RCC_APB2Periph_GPIOB 
#define  BSP_LED_RCC_APB2Periph1              RCC_APB2Periph_GPIOC               
#define  BSP_LED_RCC_APB2Periph2              RCC_APB2Periph_GPIOD
#define  BSP_LED_RCC_APB2Periph3              RCC_APB2Periph_GPIOD 

#define  LED_ON	                                 1
#define  LED_OFF	                               0

#define LED1(a)	if (a)	\
					GPIO_SetBits(BSP_LED_GPIO1,BSP_LED_PIN1);\
					else		\
					GPIO_ResetBits(BSP_LED_GPIO1,BSP_LED_PIN1)

#define LED2(a)	if (a)	\
					GPIO_SetBits(BSP_LED_GPIO2,BSP_LED_PIN2);\
					else		\
					GPIO_ResetBits(BSP_LED_GPIO2,BSP_LED_PIN2)

#define LED3(a)	if (a)	\
					GPIO_SetBits(BSP_LED_GPIO3,BSP_LED_PIN3);\
					else		\
					GPIO_ResetBits(BSP_LED_GPIO3,BSP_LED_PIN3)
					
#define LED4(a)	if (a)	\
					GPIO_SetBits(BSP_LED_GPIO4,BSP_LED_PIN4);\
					else		\
					GPIO_ResetBits(BSP_LED_GPIO4,BSP_LED_PIN4)

#define	Hi(p,i)				{p->BSRR =i;}				
#define Lo(p,i)				{p->BRR	=i;}			
#define Toggle(p,i)		{p->ODR ^=i;}			

#define LED1_TOGGLE		Toggle(BSP_LED_GPIO1,BSP_LED_PIN1)
#define LED1_OFF	  		Lo(BSP_LED_GPIO1,BSP_LED_PIN1)
#define LED1_ON			Hi(BSP_LED_GPIO1,BSP_LED_PIN1)

#define LED2_TOGGLE		Toggle(BSP_LED_GPIO2,BSP_LED_PIN2)
#define LED2_OFF 			Lo(BSP_LED_GPIO2,BSP_LED_PIN2)
#define LED2_ON			Hi(BSP_LED_GPIO2,BSP_LED_PIN2)

#define LED3_TOGGLE		Toggle(BSP_LED_GPIO3,BSP_LED_PIN3)
#define LED3_OFF 			Hi(BSP_LED_GPIO3,BSP_LED_PIN3)
#define LED3_ON			Lo(BSP_LED_GPIO3,BSP_LED_PIN3)

#define LED4_TOGGLE		Toggle(BSP_LED_GPIO4,BSP_LED_PIN4)
#define LED4_OFF 			Hi(BSP_LED_GPIO4,BSP_LED_PIN4)
#define LED4_ON			Lo(BSP_LED_GPIO4,BSP_LED_PIN4)


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_LED_Init(void);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of module include.                               */
