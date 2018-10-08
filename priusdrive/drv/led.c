#include "led.h"

uint8_t LED_State = 0;

void LED_Initialise(void)
{
	/*
	 * LED GPIO Initialisation
	 */

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//tfp_printf("LED driver initialised!\r\n");
}

void LED_On(void)
{
	/*
	 * Turn LED on
	 */

	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	LED_State = 1;
}

void LED_Off(void)
{
	/*
	 * Turn LED off
	 */

	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	LED_State = 0;
}

void LED_Toggle(void)
{
	/*
	 * Toggle LED
	 */
	if(LED_State == 1)
	{
		LED_State = 0;
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	} else if(LED_State == 0){
		LED_State = 1;
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
}

void LED_Blink(char pattern)
{
	/*
	 * TODO: LED Blinker based on bit shifting
	 */
}
