#ifndef __CAN_H
#define	__CAN_H

#include "stm32f10x.h"
#include "stm32f10x_can.h"

static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);
static void CAN_Mode_Config(void);
static void CAN_Filter_Config(void);
void CAN_Config(void);
void CAN_SetMsg(int ctrl1);
void CAN_SetData(int ID,int data[8]);
void CAN_SetData_n(int n,int ID,int data[8]);
#endif
