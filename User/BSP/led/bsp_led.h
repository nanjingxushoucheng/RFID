#ifndef __LED_H_
#define	__LED_H_



#include "stm32f10x.h"
 
 
void LED_GPIO_Config(void);
void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


#endif /* __LED_H_ */

