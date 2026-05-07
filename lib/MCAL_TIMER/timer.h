#ifndef TIMER_H
#define TIMER_H

#include "std_types.h"

// PWM channels (OC1A / OC1B)
#define TIMER1_CHANNEL_A 0
#define TIMER1_CHANNEL_B 1


void Timer1_PWM_Init(void);
void Timer1_SetDuty(uint8 channel, uint8 duty); //0-100%



#endif