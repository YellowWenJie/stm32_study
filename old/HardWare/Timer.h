#include "stm32f10x.h"

#ifndef __Timer_H
#define __Timer_H

void Timer_Init(void);
void TIM2_IRQHandler(void);
uint16_t Timer_GetCounter(void);

#endif
