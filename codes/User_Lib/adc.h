#ifndef _ADC_H
#define _ADC_H


#include "inc.h"

void ADC1_Init(void);
void ADC2_Init(void);
void stick_scan(FunctionalState NewState);
float Get_battery_volt(void);  
u16 Get_ADC_Value(ADC_TypeDef* ADCx, uint8_t ADC_Channel);
void ADCx_Init(void);
#endif

