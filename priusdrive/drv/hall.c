//INCLUDES
#include "hall.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"

//uint8_t Hall_GetPosition(void);

void Hall_Initialise(void)
{
	/*
	 * Hall sensor initialisation
	 */

	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//Enable clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	//Configure GPIO as input pullup
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//Configure NVIC interrupt channel
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);

	EXTI_InitStruct.EXTI_Line = EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	//tfp_printf("Hall driver initialised!\r\n");
}

void Hall_DisableInterrupt(void)
{
	/*
	 * Disable hall sensor interrupt
	 */
	NVIC_DisableIRQ(EXTI9_5_IRQn);
}

void Hall_EnableInterrupt(void)
{
	/*
	 * Enable hall sensor interrupt
	 */
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTI9_5_IRQHandler(void)
{
	/*
	 * Interrupt handler
	 */

    if ((EXTI_GetITStatus(EXTI_Line7) | EXTI_GetITStatus(EXTI_Line8) | EXTI_GetITStatus(EXTI_Line9)) != RESET) {

    	//Clear pending interrupts
    	EXTI_ClearITPendingBit(EXTI_Line7);
    	EXTI_ClearITPendingBit(EXTI_Line8);
    	EXTI_ClearITPendingBit(EXTI_Line9);

    	uint8_t hallpos = Hall_GetPosition();
    	tfp_printf("%d\r\n", hallpos);

        //Commutate motor
    	//Inverter_Commutate(hallpos);
    }
}

uint8_t Hall_GetPosition(void) {
	return (uint8_t) ((GPIO_ReadInputData(GPIOB) & (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9)) >> 7);
}
