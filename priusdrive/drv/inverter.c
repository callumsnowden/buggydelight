//INCLUDES
#include "inverter.h"

//VARIABLES
static const uint8_t COMM_TABLE[8][3] = {
		//U, V, W
		{0, 0, 0},
		{0, 0, 1},
		{0, 1, 1},
		{0, 1, 0},
		{1, 1, 0},
		{1, 0, 0},
		{1, 0, 1},
		{1, 1, 1}
};

static bool Inverter_State = false;

uint16_t PWM_PERIOD = 0;

static void Inverter_Enable(void);
static void Inverter_Disable(void);

void Inverter_Initialise(void)
{
	/*
	 * Inverter GPIO & timer initialisation routine
	 */

	//Calculate appropriate ARR value from desired frequency
	PWM_PERIOD = SYSTEM_CLOCK / PWM_FREQUENCY;

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//Initialise timer outputs
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Initialise GPIOA Pin 7 as enable output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//Timebase configuration
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0; //Set initial output to off

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; /// !!!
	//TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; /// !!!

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;

	/*
	 * DeadTime[ns] = value * (1/SystemCoreFreq) (on 72MHz: 7 is 98ns)
	 * In this case, we do not care for deadtime as the Prius inverter has that in hardware (apparently o.o)
	 */
	TIM_BDTRInitStructure.TIM_DeadTime = 0;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;

	//Configure break input
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;

	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

	//Enable timer & PWM outputs
	TIM_Cmd(TIM1, ENABLE);

	Inverter_SetState(true);

	//tfp_printf("Inverter configured and enabled!\r\n");
}


bool Inverter_GetState(void)
{
	/*
	 * Return current inverter state
	 * Could contain some error handling based on inputs to automatically flip the state
	 */

	return Inverter_State;
}

void Inverter_SetState(bool state)
{
	/*
	 * Set current inverter state
	 */

	Inverter_State = state;

	if(Inverter_State)
	{
		Inverter_Enable();
	} else if(!Inverter_State)
	{
		Inverter_Disable();
	} else
	{
		//The world implodes. You die!
	}
}

void Inverter_Enable(void)
{
	/*
	 * Inverter & timer enabling
	 */

	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	Inverter_SetRawPWM(0);

	GPIO_ResetBits(GPIOA, GPIO_Pin_7);

	Inverter_State = true;
}

void Inverter_Disable(void)
{
	/*
	 * Inverter & timer disabling
	 */

	GPIO_SetBits(GPIOA, GPIO_Pin_7);

	Inverter_SetRawPWM(0);

	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);

	TIM_CtrlPWMOutputs(TIM1, DISABLE);

	LED_Off();

	Inverter_State = false;
}

void Inverter_SetRawPWM(uint16_t pwm)
{
	/*
	 * PWM set routine, to simplify setting PWM values
	 */

	uint16_t pwm_value = pwm;

	if(pwm_value > PWM_PERIOD) pwm_value = PWM_PERIOD;
	if(pwm_value < 0) pwm_value = 0;

	TIM1->CCR1 = pwm_value;
	TIM1->CCR2 = pwm_value;
	TIM1->CCR3 = pwm_value;
}

void Inverter_SetPWM(uint8_t percent)
{
	/*
	 * Set inverter PWM percentage from 0-100%
	 */
	uint16_t ccr_value = (PWM_PERIOD / 100) * percent;
	Inverter_SetRawPWM(ccr_value);
}

void Inverter_Commutate(uint8_t step)
{
	/*
	 * Output commutation
	 */

	if(Inverter_State)
	{
		/*uint8_t step_copy = step;
		if(step_copy >= 6) step_copy = 5;
		if(step_copy < 0) step_copy = 0;
		*/

		if(step == 0 || step == 7) LED_On();

		uint8_t BU = COMM_TABLE[step][0];
		uint8_t BV = COMM_TABLE[step][1];
		uint8_t BW = COMM_TABLE[step][2];

		//Disable unneeded channels
		if(!BU) TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
		if(!BV) TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
		if(!BW) TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);

		#ifdef PWM_NORMAL
		if(BU)
		{
			TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
			TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
		}

		if(BV)
		{
			TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
			TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
		}

		if(BW)
		{
			TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
			TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
		}
		#endif

		#ifdef PWM_INVERTED
		if(BU)
		{
			TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM2);
			TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
		}

		if(BV)
		{
			TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM2);
			TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
		}

		if(BW)
		{
			TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM2);
			TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
		}
		#endif

		LED_Off();
	}
}
