#ifndef _TIM2_H
#define _TIM2_H

#include "stm32f10x.h"  

extern volatile uint32_t TimingDelay;

void TIM2_Init(void);

void Delay_ms(uint32_t ms);

#endif /* _TIM2_H */
