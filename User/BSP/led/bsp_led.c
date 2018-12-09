#include <includes.h>
#include "bsp_led.h"

 void LED_GPIO_Config(void)
 {
	 	GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOC , ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		/*设置引脚速率为50MHz */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		/* 关闭所有led灯	*/
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
		/* 关闭所有led灯	*/
		GPIO_SetBits(GPIOC, GPIO_Pin_3);

 }

 void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
			OS_ERR      err;
	 		GPIO_ResetBits(GPIOx, GPIO_Pin);
			OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
			GPIO_SetBits(GPIOx, GPIO_Pin);
 }

/*********************************************END OF FILE**********************/
