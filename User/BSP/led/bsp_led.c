#include <includes.h>
#include "bsp_led.h"

 void LED_Config(void)
 {
	 	GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOC , ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		/*������������Ϊ50MHz */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOC, GPIO_Pin_3);

 }

 void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
			OS_ERR      err;
	 		GPIO_ResetBits(GPIOx, GPIO_Pin);
			OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
			GPIO_SetBits(GPIOx, GPIO_Pin);
 }

 