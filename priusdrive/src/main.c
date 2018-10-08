/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
#include "stdint.h"
#include "main.h"

void SysTick_Handler (void)
 {
	/*
	if(comm_pos == 7)
	{
		comm_pos = 1;
		//Inverter_SetState(false);
	}
	*/

	Inverter_Commutate(comm_pos);

	/*
	uint8_t hallpos = Hall_GetPosition();
	printf("C:%d, H:%d\r\n\r\n", comm_pos, hallpos);

	comm_pos++;
	*/

	/*
	if(Systick_Enabled == 1)
	{
		if(count <= 70)
		{
			count = 70;
			//Systick_Enabled = 0;
			//TIM_Cmd(TIM3, DISABLE);
			//Hall_EnableInterrupt();
			//printf("Closed loop!\r\n");
		}

		TIM_SetAutoreload(TIM3, count);

		count--; //75
	}
	*/



	//printf("Count: %d\r\n", count);

 }

int main(void)
{
	SysTick_Init(15);

	UART_Initialise();
	init_printf(NULL, UART_putc);

	Inverter_Initialise();
	LED_Initialise();
	Hall_Initialise();
	Hall_DisableInterrupt();
	//Hall_EnableInterrupt();
	Inverter_SetPWM(70);
	//tfp_printf("Done init!");

	uint16_t FREQUENCY_STEPS = 70; //Frequency divisions
	uint16_t FREQUENCY_TOTAL = 10000; //Max commutation frequency
	uint32_t TIMER_FREQUENCY = FREQUENCY_STEPS*FREQUENCY_TOTAL;
	uint16_t TIMER_PRESCALE = (64000000/TIMER_FREQUENCY)-1;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = 6399;//TIMER_PRESCALE; //10kHz
	TIM_TimeBaseStructure.TIM_Period = count;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3, ENABLE);

	while(1)
	{
	}

}

void Commutate(void)
{
	if(comm_pos == 7)
	{
		comm_pos = 1;
	    //Inverter_SetState(false);
	}

	Inverter_Commutate(comm_pos);

	//uint8_t hallpos = Hall_GetPosition();
	//printf("C:%d, H:%d\r\n\r\n", comm_pos, hallpos);

	comm_pos++;
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) /*Check the TIM3 update interrupt occurs or not*/
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /*Remove TIMx update interrupt flag */

        Commutate();

        LED_Toggle();
    }
}

void SysTick_Init (uint16_t frequency)
{
   RCC_ClocksTypeDef RCC_Clocks;
   RCC_GetClocksFreq (&RCC_Clocks);
   SYSTEM_CLOCK = RCC_Clocks.HCLK_Frequency;
   (void) SysTick_Config (RCC_Clocks.HCLK_Frequency / frequency);
}
