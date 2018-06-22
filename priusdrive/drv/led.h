#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void LED_Initialise(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void LED_Blink(char pattern);

extern void tfp_printf(char *fmt, ...);
