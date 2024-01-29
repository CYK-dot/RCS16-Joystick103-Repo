#ifndef _TIMER_H
#define _TIMER_H

#include "inc.h"

void TIMx_Init(TIM_TypeDef* TIMx, u16 count,u16 _div, u8 pri);
void Connect_Indicate(FunctionalState NewState);



#endif

