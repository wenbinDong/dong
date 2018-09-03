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
* Filename      : bsp_led.c
* Version       : V1.00
* Programmer(s) : WEN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_led1.h" 



/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/


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

static void BSP_LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                       /* Define a GPIO_InitTypeDef                   */
		
	RCC_APB2PeriphClockCmd(BSP_LED_RCC_APB2Periph1 \
	                       |BSP_LED_RCC_APB2Periph2 \
	                       |BSP_LED_RCC_APB2Periph3 \
	                       |BSP_LED_RCC_APB2Periph4, ENABLE);     /* Open GPIOB,GPIOD peripheral clock           */
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           /* Initialize general push-pull output mode    */  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          /* IO speed 50MHz                              */ 
	
	GPIO_InitStructure.GPIO_Pin = BSP_LED_PIN1;	                 /* Choose GPIOB IO                             */  
	GPIO_Init(BSP_LED_GPIO1, &GPIO_InitStructure);	                   /* Initialize GPIOB5                           */
  
	GPIO_InitStructure.GPIO_Pin = BSP_LED_PIN2;                  /* Choose GPIOD IO                             */	
  GPIO_Init(BSP_LED_GPIO2, &GPIO_InitStructure);	                   /* Initialize GPIOD3                           */
  
	GPIO_InitStructure.GPIO_Pin = BSP_LED_PIN3;                  /* Choose GPIOD IO                             */		
  GPIO_Init(BSP_LED_GPIO3, &GPIO_InitStructure);                     /* Initialize GPIOD6                           */			  
  
	GPIO_InitStructure.GPIO_Pin = BSP_LED_PIN4;                  /* Choose GPIOD IO                             */		
  GPIO_Init(BSP_LED_GPIO4, &GPIO_InitStructure);                     /* Initialize GPIOD6                           */
	
  GPIO_ResetBits(GPIOD, BSP_LED_PIN1|BSP_LED_PIN2|BSP_LED_PIN3|BSP_LED_PIN4);	             /* Close led2¡¢led3 */
}


/*
*********************************************************************************************************
*                                               BSP_LED_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void BSP_LED_Init(void)
{
	BSP_LED_GPIO_Config();
}
/*************************END OF FILE******************************/

