//INCLUDES
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stdbool.h"

extern void tfp_printf(char *fmt, ...);

extern uint32_t SYSTEM_CLOCK;

//DEFINES
#define PWM_FREQUENCY 6400

//PWM Mode
//#define PWM_NORMAL
#define PWM_INVERTED

//Functions
void Inverter_Initialise(void);

bool Inverter_GetState(void);
void Inverter_SetState(bool state);

void Inverter_SetRawPWM(uint16_t pwm);
void Inverter_SetPWM(uint8_t percent);
void Inverter_Commutate(uint8_t step);

extern void LED_On(void);
extern void LED_Off(void);

/*
 * NOTES:
 * - MSDN pin goes to PA7, to enable drive pull it low and vice versa
 */
