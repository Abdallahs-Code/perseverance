#ifndef TIMER_H
#define TIMER_H

#include "std_types.h"

// PWM channels (OC1A / OC1B)
#define TIMER1_CHANNEL_A 0
#define TIMER1_CHANNEL_B 1


/* Timer 1 - Motors PWM */
void Timer1_PWM_Init(void);
void Timer1_SetDuty(uint8 channel, uint8 duty); //0-100%

/* Timer 3 Functions - Used for Ultra Sonic */
void Timer3_Init(void);
uint16 Timer3_Now(void);
uint16 Timer3_Elapsed(uint16 start);

#ifdef __cplusplus
}
#endif

#endif