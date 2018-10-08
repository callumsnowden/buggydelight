/*
 * main.h
 *
 *  Created on: 28 Apr 2018
 *      Author: Callum
 */

#ifndef MAIN_H_
#define MAIN_H_

//Includes
#include "stdbool.h"
#include "stdint.h"
#include "../drv/inverter.h"
#include "../drv/led.h"
#include "../drv/hall.h"
#include "../drv/uart.h"
#include "../drv/printf.h"

#define NULL (void *)0

extern uint8_t Hall_GetPosition(void);

void SysTick_Init(uint16_t frequency);

uint8_t Systick_Enabled = 1;

uint8_t comm_pos = 0;

uint8_t count = 150;

uint32_t SYSTEM_CLOCK = 0;

#endif /* MAIN_H_ */
