#include "stdint.h"

void Hall_Initialise(void);
void Hall_DisableInterrupt(void);
void Hall_EnableInterrupt(void);
uint8_t Hall_GetPosition(void);

extern void Inverter_Commutate(uint8_t step);
extern void tfp_printf(char *fmt, ...);
