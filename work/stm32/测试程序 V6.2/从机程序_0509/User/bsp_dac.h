#ifndef __DAC_H
#define	__DAC_H


#include "stm32f10x.h"


//#define REF_DAC	187			//refer to 3	DAC output 0.137V		=>  resval 1.6mO, result is 8.5A      define in bsp_dac.h document
//#define REF_DAC1	207			//refer to 3	DAC output 0.152V		=>  resval 1.6mO, result is 9.5A       define in bsp_dac.h document
#define REF_DAC	  88*5			//refer to 3	DAC output 0.137V		=>  resval 1.6mO, result is 15A      define in bsp_dac.h document
#define REF_DAC1	88*10			//refer to 3	DAC output 0.152V		=>  resval 1.6mO, result is 20A       define in bsp_dac.h document

void DAC_Mode_Init(void);
void DAC_Output(u16 value);

#endif /* __DAC_H */

